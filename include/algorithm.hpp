#pragma once

#ifndef EZ_ALGORITHM_HPP
#define EZ_ALGORITHM_HPP

#include <iterator>
#include <stack>
#include <utility>
#include <cassert>
namespace ez
{
    template<typename Iter>
    using tag_t = typename  std::iterator_traits<Iter>::iterator_category;

    template<typename Iter>
    constexpr bool is_input_iterator_v = std::is_convertible_v<tag_t<Iter>,std::input_iterator_tag>;

    
    template<typename Iter>
    constexpr bool is_forward_iterator_v = std::is_convertible_v<tag_t<Iter>,std::forward_iterator_tag>;

    
    template<typename Iter>
    constexpr bool is_bidirectional_iterator_v = std::is_convertible_v<tag_t<Iter>,std::bidirectional_iterator_tag>;

    
    template<typename Iter>
    constexpr bool is_random_access_iterator_v = std::is_convertible_v<tag_t<Iter>,std::random_access_iterator_tag>;


    template<typename Iter>
    using diff_t = typename std::iterator_traits<Iter>::difference_type;


    template<typename Iter>
    diff_t<Iter> distance(Iter first,Iter last){
        if constexpr(is_random_access_iterator_v<Iter>){
            return last - first;
        } else {
            diff_t<Iter> count = 0;
            for(;first != last; ++first){
                ++count;
            }
            return count;
        }
    }

    template<typename Iter, typename T>
    Iter lower_bound(Iter first, Iter last,const T& val){
        
        diff_t<Iter> count = distance(first,last);
        diff_t<Iter> step ;
        Iter it;

        while(count > 0){
            it = first;
            step = count /2;
            std::advance(it,step);
            if(*it < val){
                first = ++it;
                count -= step+1;
            }else{
                count = step;
            }
        }
        return first;
    }

    template<typename Iter,typename T>
    Iter upper_bound(Iter first,Iter last,const T& val){
        diff_t<Iter> count = distance(first,last);
        diff_t<Iter> step ;
        Iter it;

        while(count > 0){
            it = first;
            step = count /2;
            std::advance(it,step);
            if(!(val < *it)){
                first = ++it;
                count -= step+1;
            }else{
                count = step;
            }
        }
        return first;
    }

    // 快速排序
    template<typename Iter>
    Iter quick_select(Iter first, Iter last){
        assert(last - first > 1);
        auto random_iterator = first + rand()%(last - first);
        std::iter_swap(first,random_iterator);
        auto key = *first;
        --last;

        while(first < last){
            while(first < last && *last >= key){
                --last;
            }
            std::iter_swap(last,first);
            while(first < last && *first <= key){
                ++first;
            }
            std::iter_swap(last,first);
        }
        return first;
    }


    template<typename Iter>
    void quick_sort(Iter first,Iter last){
        if(last - first > 1){
            auto it = quick_select(first,last);
            quick_sort(first,it);
            quick_sort(it+1,last);
        }
    }

    template<typename Iter>
    void quick_sort_v2(Iter first,Iter last){
        std::stack<std::pair<Iter,Iter>> s;
        if(last - first > 1){
            s.push({first,last});
        }
        while(!s.empty()){
            auto [l,r] = s.top();
            s.pop();
            auto it = quick_select(l,r);
            if(it - l > 1){
                s.push({l,it});
            }
            if(r - (it+1) > 1){
                s.push({it+1,r});
            }
        }
    }


} // namespace ez


#endif