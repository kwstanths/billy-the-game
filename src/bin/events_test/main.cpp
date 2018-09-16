#include <iostream>
#include <Windows.h>

#include "game_engine/utility/Events.hpp"

namespace utl = game_engine::utility;

class TestClass1 {
public:
    TestClass1(utl::EventDispatcher * e) {
        e_ = e;
        object_id_ = reinterpret_cast<intptr_t>(this);

        e_->RegisterEvent(0);
        e_->RegisterEvent(1);
        e_->SubscribeToEvent(0, object_id_, std::bind(&TestClass1::Test, this));
    }

    void Step() {
        e_->NotifyEvent(1);
    }

    void Test() {
        std::cout << "EVENT 0" << std::endl;
    }

private:
    intptr_t object_id_;
    utl::EventDispatcher * e_;
};

class TestClass2 {
public:
    TestClass2(utl::EventDispatcher * e) {
        e_ = e;
        object_id_ = reinterpret_cast<intptr_t>(this);

        e_->RegisterEvent(0);
        e_->RegisterEvent(1);
        e_->SubscribeToEvent(1, object_id_, std::bind(&TestClass2::Test, this));
    }

    void Step() {
        e_->NotifyEvent(0);
    }

    void Test() {
        std::cout << "EVENT 1" << std::endl;
    }

private:
    intptr_t object_id_;
    utl::EventDispatcher * e_;
};

class TestClass3 {
public:
    TestClass3(utl::EventDispatcher * e) {
        e_ = e;
        object_id_ = reinterpret_cast<intptr_t>(this);

        e_->RegisterEvent(1);
        e_->SubscribeToEvent(1, object_id_, std::bind(&TestClass3::Test, this));
    }

    void Step() {
        
    }

    void Test() {
        std::cout << "EVENT 1" << std::endl;
    }

private:
    intptr_t object_id_;
    utl::EventDispatcher * e_;
};


int main(int argc, char ** argv){
    
    utl::EventDispatcher e(10);
    TestClass1 one(&e);
    TestClass2 two(&e);
    TestClass2 three(&e);

    for (;;) {
        e.Dispatch();
        
        two.Step();
        one.Step();
        
        Sleep(200);
    }

}
