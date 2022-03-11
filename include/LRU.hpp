#pragma once 
#ifndef EZ_LRU_HPP
#define EZ_LUR_HPP

#include <unordered_map>
#include <cassert>
#include <iostream>

namespace ez
{
    inline namespace v1{
    //单链表实现
    class LRU_cache
    {
        struct node
        {
            node(int k,int v):key(v),val(v),next(nullptr){}
            int key ;
            int val ;
            node* next ; 
        };
    public:    
        node m_tail;
        node m_free;
        node* pre_first;
        int m_capacity;
        std::unordered_map<int,node*> m_map;

    public:
        explicit LRU_cache(int cap):m_tail(0,0),m_free(0,0),pre_first(&m_tail),m_capacity(cap),m_map(){
            add_free_node(cap);
        }

        ~LRU_cache(){
            destroy_list(m_tail);
            destroy_list(m_free);
        }

        void destroy_list(node& list){
            auto head = list.next;
            while(head != nullptr){
                auto next = head->next;
                delete head;
                head = next;
            }
            list.next = nullptr;
        }

        void add_free_node(int size){
            auto cur = &m_free;
            for(int i =0 ; i < size; ++i){
                cur->next = new node(0,0);
                cur = cur->next;
            }
        }
        bool has_free_node() const{
            return m_free.next != nullptr;
        }  

        node* get_free_node()
        {
            auto cur = m_free.next;
            m_free.next = cur->next;
            cur->next = nullptr;
            return cur;
        }

        node* release_node(node* pre){
            //释放出当前结点
            auto cur =  pre->next;
            auto next = cur->next;
            pre->next = next;
            cur->next = nullptr;
            m_map.erase(cur->key);
            //如果当前点位pre_first,需要更新
            if(cur == pre_first){
                pre_first = pre;
            }
            //更新下个结点中的map的值为pre
            if(next != nullptr){
                m_map[next->key] = pre;
            }
            return cur;
        }

        int get(int key) 
        {
            auto it = m_map.find(key);
            if(it != m_map.end()){
                auto pre = it->second;
                auto cur = release_node(pre);
                move_to_first(cur);
                return cur->val;
            }
            return -1;
        }

        

        void put(int key,int val){
           auto it = m_map.find(key);
           if(it != m_map.end()){
                auto pre = it->second;
                auto cur = release_node(pre);
                move_to_first(cur);
                cur->val = val;
           }else{
               node* n = nullptr;
               if(has_free_node()){
                   n = get_free_node();
               }else{
                   n = release_node(&m_tail);
               }
               n->key = key;
               n->val = val;
               move_to_first(n);
           }
        }


        void move_to_first(node* cur){
            if(pre_first->next == nullptr){
                pre_first->next = cur;
            }else{
                auto first_node = pre_first->next;
                first_node->next = cur;
                pre_first = first_node;
            }
            
            m_map.emplace(cur->key,pre_first);
        }

    };

    }//namespace ze::v1

    namespace v2
    {

    //双向链表
    class  LRU_cache
    {
        struct node
        {
            node() =default;
            node(int k,int v):key(k),val(v){}

            void reset_prev_next(){
                prev = nullptr;
                next = nullptr;
            }

            void set(int k,int v){
                key = k;
                val= v;
            }

            int key{0};
            int val{0};
            node* prev{nullptr};
            node* next{nullptr};
        };

        std::unordered_map<int,node*> m;
        node head;
        int capacity;

        node* get_last_node(){
            auto n = head.prev;
            assert(n != &head);
            release_node(n);
            m.erase(n->key);
            return n;
        }

        void release_node(node* n){
            auto pre = n->prev;
            auto next = n->next;
            pre->next = next;
            next->prev = pre;
            n->reset_prev_next();
        }

        node* get_new_node(){
            auto n = new node();
            ++head.val;
            return n;
        }

        void move_to_first(node* n)
        {
            auto next = head.next;
            head.next = n;
            n->next = next;
            n->prev = &head;
            next->prev = n;
        }

    public:
        LRU_cache(int cap):m(),head(),capacity(cap){
            head.next = &head;
            head.prev = &head;
        }

        void set(int key, int value){
            auto it = m.find(key);
            if(it != m.end()){
                auto n = it->second;
                n->val = value;
                release_node(n);
                move_to_first(n);
            }else{
                node* n = nullptr;
                if(head.val < capacity){
                    n = get_new_node();
                }else{
                    n = get_last_node();
                }
                n->set(key,value);
                move_to_first(n);        
                m[key] = n;
            }
        }

        int get(int key){
            auto it = m.find(key);
            if(it == m.end()){
                return -1;
            }else{
                auto n = it->second;
                release_node(n);
                move_to_first(n);
                return n->val;
            }
        }
        
    };

    } // namespace v2
    


} // namespace ze


#endif //EZ_LRU_HPP