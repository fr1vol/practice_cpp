#pragma once

#ifndef EZ_SHARED_PTR_HPP
#define EZ_SHARED_PTR_HPP

#include <compressed_pair.hpp>

namespace ez
{


    class count_base
    {
    public:

        constexpr count_base() noexcept :owner(1),weak(1){}

        count_base(const count_base&) = delete;
        count_base& operator=(const count_base&) = delete;

        virtual ~count_base() noexcept = default;

        void increase_owner() noexcept {
            ++owner;
        }
        
        void increase_weak() noexcept{
            ++weak;
        }

        void decrease_owner() noexcept {
            --owner;
            if(owner == 0){
                destroy();
                decrease_weak();
            }
        }

        void decrease_weak() noexcept{
            --weak;
            if(weak == 0){
                destroy_this();
            }
        }

        int owner_count() const {
            return owner;
        }

        int weak_count() const {
            return weak;
        }

        virtual void destroy() noexcept = 0;
        virtual void destroy_this() noexcept = 0;
    private:
        int owner;
        int weak;
    };


    template<typename T>
    class ref_count : public count_base
    {
    public:
        explicit ref_count(T* p):count_base(),resource(p){}
    private:
        void destroy() noexcept override
        {
            delete resource;
        }

        void destroy_this() noexcept override
        {
            delete this;
        }

        T* resource;
    };


    template<typename T, typename Deleter>
    class ref_count_del : public count_base 
    {
    public:
        ref_count_del(T* p, Deleter d):count_base(),resource(use_first_t{},std::move(d),p){}

    private:
        void destroy() noexcept override{
            auto ptr = resource.val2;
            resource.get_first()(ptr);
        }

        void destroy_this() noexcept override{
            delete this;
        }

    private:
        compressed_pair<Deleter,T*> resource; 
    };

    template<typename T>
    class shared_ptr
    {
    public:
        using pointer = T*;    
        using element_type = T;

        constexpr shared_ptr() noexcept :ptr(nullptr),control_block(nullptr){}
        constexpr shared_ptr(std::nullptr_t) noexcept :shared_ptr(){}

        template<typename U>
        explicit shared_ptr(U* p):ptr(p),control_block(new ref_count<U>(p)){}

        template<typename U,typename Deleter>
        shared_ptr(U* p,Deleter d):ptr(p),control_block(new ref_count_del<U,Deleter>(p,std::move(d))){}

    private:

        template<typename U>
        friend class shared_ptr;

        template<typename U>
        void copy_construct_from(const shared_ptr<U>& other){
            
            if(other.control_block != nullptr){
                other.control_block->increase_owner();
            }
            ptr = other.ptr;
            control_block = other.control_block;
        }

        template<typename U>
        void move_construct_from(shared_ptr<U>&& other){
            ptr = other.ptr;
            control_block = other.control_block;
            other.ptr = nullptr;
            other.control_block = nullptr;
        }
    public:
        shared_ptr(const shared_ptr& other){
            copy_construct_from(other);
        }

        template<typename U>
        shared_ptr(const shared_ptr<U>& other){
            copy_construct_from(other);
        }

        shared_ptr(shared_ptr&& other){
            move_construct_from(std::move(other));
        }

        template<typename U>
        shared_ptr(shared_ptr&& other){
            move_construct_from(std::move(other));
        }

        shared_ptr& operator=(const shared_ptr& other){
            shared_ptr(other).swap(*this);
            return *this;
        }

        shared_ptr&  operator=(shared_ptr&& other){
            shared_ptr(std::move(other)).swap(*this);
            return *this;
        }

        ~shared_ptr(){
            if(control_block != nullptr){
                control_block->decrease_owner();
            }
        }

        void swap(shared_ptr& other){
            using std::swap;
            swap(ptr,other.ptr);
            swap(control_block,other.control_block);
        }

        element_type* get() const noexcept{
            return ptr;
        }

        element_type& operator*() const noexcept{
            return *ptr;
        }

        element_type* operator->() const noexcept{
            return ptr;
        }

        explicit operator bool() const noexcept{
            return ptr != nullptr;
        }

        void reset(){
            shared_ptr().swap(*this);
        }
        void reset(std::nullptr_t){
            reset();
        }

        template<typename U>
        void reset(U* p = nullptr){
            shared_ptr(p).swap(*this);
        }

        template<typename U, typename Deleter>
        void reset(U* p, Deleter d){
            shared_ptr(p,d).swap(*this);
        }

        long use_count() const {
            if(control_block != nullptr){
                return control_block->owner_count();
            }
            return 0;
        }

    private:
        element_type*  ptr;
        count_base* control_block; 
    };



}// namespace ez

#endif //EZ_SHARED_PTR_HPP