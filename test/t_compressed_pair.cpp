#include <catch.hpp>
#include <compressed_pair.hpp>

#include <type_traits>

TEST_CASE( "test compressed_pair class","[compressed_pair]")
{
    SECTION("init with empty class"){
        struct empty_class {};
        ez::compressed_pair<empty_class,int> a{ez::not_use_t{},2};
        REQUIRE( sizeof(a) == sizeof(int));
        REQUIRE( a.val2 == 2);
    }

    SECTION("init with non-empty class"){
        struct non_empty_class {int a;};
        ez::compressed_pair<non_empty_class,int> a{ez::use_first_t{},non_empty_class{2},4};
        REQUIRE( sizeof(a) == sizeof(int)*2);
        
        REQUIRE( a.get_first().a == 2);
        REQUIRE( a.val2 == 4);
    }


}
