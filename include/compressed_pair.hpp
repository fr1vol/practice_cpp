#pragma once

#ifndef EZ_COMPRESSED_PAIR_HPP
#define EZ_COMPRESSED_PAIR_HPP

#include <type_traits>

namespace ez
{

    struct use_first_t {explicit use_first_t() =default;};
    struct not_use_t   {explicit not_use_t() =default;};
    

    template<typename T1,typename T2, bool = std::is_empty_v<T1> && !std::is_final_v<T1>>
    class compressed_pair final : private T1
    {
    public:
        T2  val2;

        template<typename... Args>
        constexpr explicit  compressed_pair(not_use_t, Args&&... args)
            noexcept(std::conjunction_v<std::is_nothrow_default_constructible<T1>, std::is_nothrow_constructible<T2,Args...>>)
            :T1(),val2(std::forward<Args>(args)...) 
        {}

        template<typename First,typename... Args>
        constexpr compressed_pair(use_first_t,First&& first_arg,  Args&&... args)
            noexcept(std::conjunction_v<std::is_nothrow_constructible<T1,First>, std::is_nothrow_constructible<T2,Args...>>)
            :T1(std::forward<First>(first_arg)),val2(std::forward<Args>(args)...) 
        {}

        constexpr T1& get_first() noexcept{
            return *this;
        }

        constexpr const T1& get_first() const noexcept{
            return *this;
        }
    };

    template<typename T1,typename T2>
    class compressed_pair<T1,T2,false> final
    {
    public:
        T1  val1;
        T2  val2;

        template<typename... Args>
        constexpr explicit  compressed_pair(not_use_t, Args&&... args)
            noexcept(std::conjunction_v<std::is_nothrow_default_constructible<T1>, std::is_nothrow_constructible<T2,Args...>>)
            :val1(),val2(std::forward<Args>(args)...) 
        {}

        template<typename First,typename... Args>
        constexpr compressed_pair(use_first_t,First&& first_arg,  Args&&... args)
            noexcept(std::conjunction_v<std::is_nothrow_constructible<T1,First>, std::is_nothrow_constructible<T2,Args...>>)
            :val1(std::forward<First>(first_arg)),val2(std::forward<Args>(args)...) 
        {}

        constexpr T1& get_first() noexcept{
            return val1;
        }

        constexpr const T1& get_first() const noexcept{
            return val1;
        }
    };


} // namespace ez

#endif //EZ_COMPRESSED_PAIR_HPP