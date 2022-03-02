#pragma once

#ifndef EZ_UNIQUE_PTR_HPP
#define EZ_UNIQUE_PTR_HPP

#include <compressed_pair.hpp>
#include <cstddef>
namespace ez
{

    template<typename T>
    class default_delete
    {
    public:
        constexpr default_delete() noexcept = default;
        void operator(T* ptr)() const {
            if(ptr != nullptr){
                delete ptr;
            }
        }
    };



    template<typename T,typename Deleter = default_delete<T>>
    class unique_ptr
    {
    public:
        using pointer = T*;
        using element_type = T;
        using deleter_type =  Deleter;

        constexpr unique_ptr() noexcept :my_pair(not_use_t{},nullptr) {}
        constexpr unique_ptr(std::nullptr_t) noexcept:unique_ptr() {}

        explicit unique_ptr(pointer p)noexcept :my_pair(not_use_t{},p) {}

        unique_ptr(pointer p,const Deleter& d) : my_pair(use_first_t{},d,p){}

        unique_ptr(pointer p, Deleter&& d):my_pair(use_first_t{},std::move(d),p);

        unique_ptr(const unique_ptr&) = delete;
        unique_ptr& operator=(const unique_ptr&) = delete;

        unique_ptr(unique_ptr&& other){
            reset(other.release());
        }
        unique_ptr& operator=(unique_ptr&& other){
            reset(other.release());
            return *this;
        }

        ~unique_ptr(){
            if(get_pointer() != nullptr){

            }
        }

        pointer release()  noexcept{
            pointer p = get_pointer();
            get_pointer() = nullptr;
            return p;
        }

        void reset(pointer p = pointer())noexcept{
            auto old_ptr = get_pointer();
            get_pointer() = p;
            if(old_ptr != nullptr){
                get_deleter()(old_ptr);
            }
        }

        void swap(unique_ptr& other) noexcept{
            using std::swap;
            swap(other.get_deleter(),this->get_deleter());
            swap(other.get_pointer(),this->get_pointer());
        }

        Deleter& get_deleter() noexcept{
            return my_pair.get_first();
        }

        pointer get() const noexcept{
            return get_pointer();
        }

        explict operator bool() const noexcept{
            return static_cast<bool>(get_pointer());
        }

        pointer operator->() const noexcept{
            return get();
        }

        typename std::add_lvalue_reference_t<T> operator* () const noexcept{
            return *get();
        }

    private:
        pointer& get_pointer(){
            return my_pair.val2;
        }

    private:
        compressed_pair<Deleter,pointer> my_pair;
    };




}// namespace ez

#endif //EZ_UNIQUE_PTR_HPP