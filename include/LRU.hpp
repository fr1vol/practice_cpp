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

        void print(){
            auto cur = m_tail.next;
            while(cur != nullptr){
                std::cout<<"("<<cur->key<<","<<cur->val<<","<<m_map[cur->key]->key<< ")->";
                cur=cur->next;
            }
            std::cout<<"nullptr \n first_node :";
            if(pre_first->next == nullptr){
                std::cout<<"nullptr \n\n";
            }else{
                cur = pre_first->next;
                std::cout<<"("<<cur->key<<","<<cur->val<<") \n\n";
            }

        }
    public:
        explicit LRU_cache(int cap):m_tail(0,0),m_free(0,0),pre_first(&m_tail),m_capacity(cap),m_map(){
            add_free_node(cap);
        }

        ~LRU_cache(){
            destroy_list(m_tail.next);
            destroy_list(m_free.next);
        }

        void destroy_list(node* head){
            while(head != nullptr){
                auto next = head->next;
                delete head;
                head = next;
            }
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
                m_map.emplace(key,pre_first);
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
                m_map.emplace(key,pre_first);
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
               m_map.emplace(key,pre_first);
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
        }

    };

    }//namespace ze::v1

    namespace v2
    {

    //双向链表
    class  LRU_cache
    {
    
    struct Node     //双向链表
    {
        Node(int val_,int key_):val(val_),key(key_),next(nullptr),pre(nullptr){}
        Node():Node(0,0){}
        int val;
        int key;
        Node* next;
        Node* pre;
    };

    std::unordered_map<int,Node*> m;   //查找
    Node head;  //双向链表头部
    Node tail;  //双向链表尾部
    int capacity;   //指示数量


    void add_free_node(int size){  //向尾部添加free_node
        auto cur = &tail;
        for(int i =0 ; i < size; ++i){
            cur->next = new Node(0,0);
            cur = cur->next;
        }
    }

    void insert_head(Node* node){   //将节点插入到开头
        assert(node != nullptr);
        auto old_first = head.next;
        head.next = node;
        node->next = old_first;
        node->pre = &head;
        old_first->pre = node;
    }

    void release_node(Node* node){  //将指定node节点提取出来
        assert(node != nullptr && node != &head &&  node != &tail);
        auto pre_node = node->pre;
        auto next_node = node->next;
        pre_node->next = next_node;
        next_node->pre = pre_node;
        node->pre  = nullptr;
        node->next = nullptr;
    }

    Node* get_last()        //获取最后一个node
    {
        assert(tail.pre != &head);
        auto n = tail.pre;
        release_node(n);
        return n;
    }
    Node* get_free()  //获取一个free node
    {
        assert(tail.next != nullptr);
        auto n =tail.next;
        tail.next = n->next;
        n->next = nullptr;
        return n;
    }
public:
    LRU_cache(int cap):m(),head(),tail(),capacity(cap) {   //构造函数添加free node
        head.next = &tail;
        tail.pre = &head;
        add_free_node(cap);
    }
    
    ~LRU_cache()     //析构函数释放管理的节点
    {
        auto n = head.next;
        while(n != &tail){
            auto next_node = n->next;
            delete n;
            n = next_node;
        }

        n = tail.next;
        while(n != nullptr){
            auto next_node = n->next;
            delete n;
            n = next_node;
        }
    }
    int get(int key) {
        if(auto it = m.find(key); it == m.end()){  //不存在节点
            return -1;
        }else{      
            auto node = it->second;     //存在，需要将node提前
            release_node(node);
            insert_head(node);
            return node->val;
        }
    }
    
    void put(int key, int value) {
        if(auto it = m.find(key); it == m.end()){   //不存在该node
            Node* node = nullptr;
            if(tail.next == nullptr){   //是否有free node，没有的化，将最后一个node拿来使用
                node = get_last();
                m.erase(node->key);
            }else{                  //获取free node
                node = get_free();  
            }
            node->key = key;
            node->val = value;
            m.emplace(key,node);
            insert_head(node);
        }else{                  //已存在node，更新并提前
            auto n = it->second;
            n->val = value;
            release_node(n);
            insert_head(n);
        }
    }
    };

    } // namespace v2
    


} // namespace ze


#endif //EZ_LRU_HPP