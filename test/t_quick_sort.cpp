#include <catch.hpp>
#include <algorithm.hpp>
#include <vector>
#include <iostream>

TEST_CASE("test quick sort function","[function]"){
    
    std::vector<int> v = {1,4,5,2,3,9,8,7,6,6,4,3,7,1,0};    
    auto s = v;
    ez::quick_sort(v.begin(),v.end());
    std::sort(s.begin(),s.end());
    REQUIRE(s == v);
}

TEST_CASE("test quick sort v2 function","[function]"){
    
    std::vector<int> v = {1,4,5,2,3,9,8,7,6,6,4,3,7,1,0};    
    auto s = v;
    ez::quick_sort_v2(v.begin(),v.end());
    std::sort(s.begin(),s.end());
    REQUIRE(s == v);
}