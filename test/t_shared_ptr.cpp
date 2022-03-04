#include <catch.hpp>
#include <shared_ptr.hpp>
#include <functional>
#include <iostream>
TEST_CASE("test count_base class","[ref_count]"){

    class test_base : public ez::count_base
    {   
    public: 
        constexpr test_base(int s):ez::count_base(),size(s){}

        void destroy() noexcept override{
            --size;
        }

        void destroy_this() noexcept override{
            size-=2;
        }

        int size ;
    };


    SECTION("default construct"){
        test_base a{5};
        REQUIRE(a.owner_count() == 1);
        REQUIRE(a.weak_count() == 1);
        REQUIRE(a.size == 5);
    }

    SECTION("increase_owner function"){
        test_base a{5};
        a.increase_owner();
        REQUIRE(a.owner_count() == 2);
        REQUIRE(a.weak_count() == 1);
        REQUIRE(a.size == 5);
    }

    SECTION("increase_weak function"){
        test_base a{5};
        a.increase_weak();
        REQUIRE(a.owner_count() == 1);
        REQUIRE(a.weak_count() == 2);
        REQUIRE(a.size == 5);
    }

    SECTION("decrease_owner function"){
        test_base a{5};
        a.increase_weak();  //now weak == 2

        a.decrease_owner();
        REQUIRE(a.owner_count() == 0);
        REQUIRE(a.weak_count() == 1); //call decrease_weak
        REQUIRE(a.size == 4); //call destroy 
    }
  

    SECTION("decrease_weak function"){
        test_base a{5};
        a.decrease_weak();
        REQUIRE(a.owner_count() == 1);
        REQUIRE(a.weak_count() == 0);
        REQUIRE(a.size == 3); //call destroy this
    }

}


TEST_CASE("test shared_ptr class","[shared_ptr]"){

    REQUIRE(sizeof(ez::shared_ptr<int>) == sizeof(void*)*2); 

    SECTION("default  construct")
    {
        ez::shared_ptr<int> p{};
        REQUIRE(!p);
        REQUIRE(p.get() == nullptr);
        REQUIRE(p.use_count() == 0);

        
        ez::shared_ptr<int> p2{nullptr};
        REQUIRE(!p2);
        REQUIRE(p2.get() == nullptr);
        REQUIRE(p2.use_count() == 0);
    }

    SECTION("construct with pointer"){
        int *a = new int(20);
        ez::shared_ptr<int> p(a);

        REQUIRE(p);
        REQUIRE(p.get() != nullptr);
        REQUIRE(p.use_count() == 1);
        REQUIRE(*p == 20);
    }

    SECTION("construct with pointer & deleter"){

        
        int *a = new int(20);
        auto f = [](int* p){delete p;};

        ez::shared_ptr<int> p(a,f);
        REQUIRE(p);
        REQUIRE(p.get() != nullptr);
        REQUIRE(p.use_count() == 1);
        REQUIRE(*p == 20);
    }

    SECTION("copy construct"){
        int *a = new int(20);
        ez::shared_ptr<int> p(a);

        ez::shared_ptr<int> p2(p);
        
        //source pointer unchanged
        REQUIRE(p);
        REQUIRE(p.get() != nullptr);
        REQUIRE(p.use_count() == 2);
        REQUIRE(*p == 20);

        REQUIRE(p2);
        REQUIRE(p2.get() != nullptr);
        REQUIRE(p2.use_count() == 2);
        REQUIRE(*p2 == 20);

    }

    SECTION("move construct"){
        int *a = new int(20);
        ez::shared_ptr<int> p(a);

        ez::shared_ptr<int> p2(std::move(p));

        //source pointer changed
        REQUIRE(!p);
        REQUIRE(p.get() == nullptr);
        REQUIRE(p.use_count() == 0);

        REQUIRE(p2);
        REQUIRE(p2.get() != nullptr);
        REQUIRE(p2.use_count() == 1);
        REQUIRE(*p2 == 20);
    }

     SECTION("copy operator"){
        int *a = new int(20);
        ez::shared_ptr<int> p(a);
        ez::shared_ptr<int> p2;

        p2 = p;
        
        //source pointer unchanged
        REQUIRE(p);
        REQUIRE(p.get() != nullptr);
        REQUIRE(p.use_count() == 2);
        REQUIRE(*p == 20);

        REQUIRE(p2);
        REQUIRE(p2.get() != nullptr);
        REQUIRE(p2.use_count() == 2);
        REQUIRE(*p2 == 20);

    }

    SECTION("move opeartor"){
        int *a = new int(20);
        ez::shared_ptr<int> p(a);
        ez::shared_ptr<int> p2;

        p2 = std::move(p);

        //source pointer changed
        REQUIRE(!p);
        REQUIRE(p.get() == nullptr);
        REQUIRE(p.use_count() == 0);

        REQUIRE(p2);
        REQUIRE(p2.get() != nullptr);
        REQUIRE(p2.use_count() == 1);
        REQUIRE(*p2 == 20);
    }

    SECTION("reset function "){
        int *a = new int(20);
        ez::shared_ptr<int> p(a);
    
        p.reset();

        REQUIRE(!p);
        REQUIRE(p.get() == nullptr);
        REQUIRE(p.use_count() == 0);

        int *b = new int(10);
        p.reset(b);
        REQUIRE(p);
        REQUIRE(p.get() != nullptr);
        REQUIRE(p.use_count() == 1);
        REQUIRE(*p == 10);

        
        p.reset(nullptr);

        REQUIRE(!p);
        REQUIRE(p.get() == nullptr);
        REQUIRE(p.use_count() == 0);

    }

    SECTION("swap function "){
        int *a = new int(20);
        ez::shared_ptr<int> p(a);

        int *a2 = new int(10);
        ez::shared_ptr<int> p2(a2);

        p.swap(p2);

        REQUIRE(p);
        REQUIRE(p.get() != nullptr);
        REQUIRE(p.use_count() == 1);
        REQUIRE(*p == 10);

        REQUIRE(p2);
        REQUIRE(p2.get() != nullptr);
        REQUIRE(p2.use_count() == 1);
        REQUIRE(*p2 == 20);

    }

}