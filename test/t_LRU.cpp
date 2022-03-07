#include <catch.hpp>
#include <LRU.hpp>
#include <iostream>

TEST_CASE("test LRU cache class","[LRU_cache]"){

    SECTION("default construct"){

        ez::LRU_cache cache(3);
        REQUIRE(cache.m_capacity == 3);
        REQUIRE(cache.m_map.empty());
        REQUIRE(cache.pre_first == &(cache.m_tail));
        REQUIRE(cache.m_tail.next == nullptr);
        cache.print();
    }

    SECTION("test put function"){
        ez::LRU_cache cache(3);

        cache.put(2,3);
        REQUIRE(cache.m_map.size() == 1);
        REQUIRE(cache.pre_first == &(cache.m_tail));
        auto cur_node = cache.pre_first->next;
        REQUIRE(cur_node->key == 2);
        REQUIRE(cur_node->val == 3);
        REQUIRE(cache.m_map[2] == cache.pre_first);
        cache.print();


        cache.put(2,4);
        REQUIRE(cache.m_map.size() == 1);
        REQUIRE(cache.pre_first == &(cache.m_tail));
        cur_node = cache.pre_first->next;
        REQUIRE(cur_node->key == 2);
        REQUIRE(cur_node->val == 4);
        REQUIRE(cache.m_map[2] == cache.pre_first);
        cache.print();

        cache.put(3,12);
        REQUIRE(cache.m_map.size() == 2);
        REQUIRE(cache.pre_first == cache.m_tail.next);
        cur_node = cache.pre_first->next;
        REQUIRE(cur_node->key == 3);
        REQUIRE(cur_node->val == 12);
        REQUIRE(cache.m_map[3] == cache.pre_first);
        cache.print();

        cache.put(4,7);
        REQUIRE(cache.m_map.size() == 3);
        REQUIRE(cache.pre_first == cache.m_map[3]->next);
        cur_node = cache.pre_first->next;
        REQUIRE(cur_node->key == 4);
        REQUIRE(cur_node->val == 7);
        REQUIRE(cache.m_map[4] == cache.pre_first);
        cache.print();

        cache.put(4,3);
        REQUIRE(cache.m_map.size() == 3);
        REQUIRE(cache.pre_first == cache.m_map[3]->next);
        cur_node = cache.pre_first->next;
        REQUIRE(cur_node->key == 4);
        REQUIRE(cur_node->val == 3);
        REQUIRE(cache.m_map[4] == cache.pre_first);
        cache.print();

    }

    SECTION("test get function"){
        ez::LRU_cache cache(3);

        cache.put(2,3);
        cache.put(3,12);
        cache.put(4,7);

        int val = cache.get(2);
        REQUIRE(val == 3);
        REQUIRE(cache.pre_first == cache.m_map[4]->next);
        auto cur_node = cache.pre_first->next;
        REQUIRE(cur_node->key == 2);
        REQUIRE(cur_node->val == 3);
        REQUIRE(cache.m_map[2] == cache.pre_first);
        cache.print();

        
        int val2 = cache.get(2);
        REQUIRE(val2 == 3);
        REQUIRE(cache.pre_first == cache.m_map[4]->next);
         cur_node = cache.pre_first->next;
        REQUIRE(cur_node->key == 2);
        REQUIRE(cur_node->val == 3);
        REQUIRE(cache.m_map[2] == cache.pre_first);
        cache.print();

        int val3 = cache.get(-1);
        REQUIRE(val3 == -1);

        int val4 = cache.get(3);
        REQUIRE(val4 == 12);
        REQUIRE(cache.pre_first == cache.m_map[2]->next);
         cur_node = cache.pre_first->next;
        REQUIRE(cur_node->key == 3);
        REQUIRE(cur_node->val == 12);
        REQUIRE(cache.m_map[3] == cache.pre_first);
        cache.print();

    }

    SECTION("test"){
        ez::LRU_cache cache(3);
        cache.put(1,1);
        cache.put(2,2);
        cache.put(3,3);
        cache.print();
        cache.get(2);
        cache.print();
    }
}