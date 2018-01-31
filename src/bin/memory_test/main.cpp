#ifdef _WIN32
#include <Windows.h>
#endif

#include <iostream>
#include <vector>

#include "debug_tools/Console.hpp"

#include "game_engine/memory/MemoryPage.hpp"
#include "game_engine/memory/ArrayAllocator.hpp"
#include "game_engine/memory/PoolAllocator.hpp"


namespace dt = debug_tools;
namespace ms = memory_subsystem;

class DummyClass {
private:
    int arg_1, arg_2;
    std::vector<int> * arr = nullptr;
public:
    DummyClass() { }
    void Init() {
        arg_1 = 1;
        arg_2 = 2;
        arr = new std::vector<int>(15, 1);
    }
    void SetOne(int a) { arg_1 = a; }
    void SetTwo(int a) { arg_2 = a; }
    int GetOne() { return arg_1; }
    int GetTwo() { return arg_2; }
    void PrintVector() {
        if (arr == nullptr) return;
        for (size_t i = 0; i < arr->size(); i++) { std::cout << arr->at(i) << " "; }
        dt::Console("\n");
    }
};

class DummyClass_2 : public DummyClass {
private:
    int arg_3, arg_4;
public:
    DummyClass_2() { }
    void Init() {
        arg_3 = 1;
        arg_4 = 2;
        DummyClass::Init();
    }
    void SetThree(int a) { arg_3 = a; }
    void SetFour(int a) { arg_4 = a; }
    int GetThree() { return arg_3; }
    int GetFour() { return arg_4; }
};


void CheckMemoryPage() {
    {
        ms::MemoryPage page(4096);

        dt::Console("Checking element access...");
        {
            ms::BYTE * start = page.Get(0);
            int * heap_int_1 = reinterpret_cast<int *>(start);
            *heap_int_1 = 5;
            dt::Console(*heap_int_1);
            dt::Console(&(*heap_int_1));

            dt::Console("\n");

            char * end = page.Get(4096 - 4);
            int * heap_int_2 = reinterpret_cast<int *>(end);
            *heap_int_2 = -1;
            dt::Console(*heap_int_2);
            dt::Console(&(*heap_int_2));

            dt::Console("\n");

            int * heap_int_3 = page.Get<int>(sizeof(int));
            *heap_int_1 = 5;
            *heap_int_3 = 6;
            dt::Console(*heap_int_1);
            dt::Console(*heap_int_3);
            dt::Console(&(*heap_int_1));
            dt::Console(&(*heap_int_3));
        }
        dt::Console("\n-------------------------------\n");
        {
            dt::Console("Checking overlapping...");
            ms::BYTE * start = page.Get(0);
            int * heap_int_1 = reinterpret_cast<int *>(start);
            int * heap_int_2 = reinterpret_cast<int *>(start + 1);

            *heap_int_2 = 5;
            dt::Console(*heap_int_1);
            dt::Console(*heap_int_2);
            dt::Console(&(*heap_int_1));
            dt::Console(&(*heap_int_2));
        }
        dt::Console("\n-------------------------------\n");
        {
            ms::BYTE * class_address = page.Get(100, sizeof(DummyClass));
            dt::Console("Checking custom types...");
            DummyClass * ptr = reinterpret_cast<DummyClass *>(class_address);
            dt::Console(ptr->GetOne());
            dt::Console(ptr->GetTwo());
            ptr->PrintVector();
            ptr->Init();
            dt::Console(ptr->GetOne());
            dt::Console(ptr->GetTwo());
            ptr->PrintVector();
        }
    }

    {
        ms::MemoryPage page(8);
        int * int_1 = page.Get<int>(0);
        int * int_2 = page.Get<int>(sizeof(int));
        if (int_1 == nullptr || int_2 == nullptr) dt::Console("FAILED, fully packed page");
        *int_1 = 17;
        *int_2 = 23;
        dt::Console(&(*int_1));
        dt::Console(&(*int_2));
        dt::Console(*int_1);
        dt::Console(*int_2);
    }

}

void CheckArrayAllocator() {
    ms::ArrayAllocator marr;
    marr.Init(16);

    {
        dt::Console("Checking element access");
        ms::BYTE * start = marr.Allocate(sizeof(int));
        int * heap_int_1 = reinterpret_cast<int *>(start);
        *heap_int_1 = 42;
        dt::Console(&(*heap_int_1));
        dt::Console(*heap_int_1);

        dt::Console("\n-------------------------------\n");

        ms::BYTE * next = marr.Allocate(sizeof(int));
        int *heap_int_2 = reinterpret_cast<int *>(next);
        *heap_int_2 = 43;
        dt::Console(&(*heap_int_2));
        dt::Console(*heap_int_2);

        dt::Console("\n-------------------------------\n");

        next = marr.Allocate(sizeof(double));
        double *heap_double_1 = reinterpret_cast<double *>(next);
        *heap_double_1 = 43.44;
        dt::Console(&(*heap_double_1));
        dt::Console(*heap_double_1);

        dt::Console("\n-------------------------------\n");

        double * heap_double_2 = marr.Allocate<double>();
        *heap_double_2 = 102.1234;
        dt::Console(&(*heap_int_1));
        dt::Console(&(*heap_int_2));
        dt::Console(&(*heap_double_1));
        dt::Console(&(*heap_double_2));
        dt::Console(*heap_int_1);
        dt::Console(*heap_int_2);
        dt::Console(*heap_double_1);
        dt::Console(*heap_double_2);
    }
    
    dt::Console("\n-------------------------------\n");

    {
        dt::Console("Checking custom types");
        DummyClass * temp_1 = marr.Allocate<DummyClass>();
        dt::Console(temp_1->GetOne());
        dt::Console(temp_1->GetTwo());
        temp_1->PrintVector();
        temp_1->Init();
        dt::Console(temp_1->GetOne());
        dt::Console(temp_1->GetTwo());
        temp_1->PrintVector();

        dt::Console("\n-------------------------------\n");

        DummyClass * temp_2 = marr.Allocate<DummyClass>();
        temp_2->Init();
        dt::Console(temp_2->GetOne());
        dt::Console(temp_2->GetTwo());
        temp_2->PrintVector();

        DummyClass_2 * temp_3 = marr.Allocate<DummyClass_2>();
        temp_3->Init();
        dt::Console(temp_3->GetOne());
        dt::Console(temp_3->GetTwo());
        dt::Console(temp_3->GetThree());
        dt::Console(temp_3->GetFour());
        temp_2->PrintVector();
    }

    dt::Console("\n-------------------------------\n");

    {
        dt::Console("Checking aligned allocate");
        char * test = marr.Allocate<char>();
        dt::Console(static_cast<void *>(&(*test)));
        dt::Console("Unaligned next int should be stored in:");
        dt::Console(static_cast<void *>(&(*test) +1));
        dt::Console(static_cast<void *>(&(*test) +4));
        dt::Console("Aligned allocation:");
        int * test_int = marr.AllocateAligned<int>(4);
        dt::Console(&(*test_int));
        dt::Console(&(*test_int) + 1);
    }

    dt::Console("Memory allocated: " + std::to_string(marr.GetBytesAllocated()) + " Bytes");
    dt::Console("Memory used: " + std::to_string(marr.GetBytesUsed()) + " Bytes");
    
}

void CheckPoolAllocator() {
    ms::PoolAllocator mpool;
    mpool.Init(20, 5);

    {
        dt::Console("Checking element allocation");
        int * mem_1 = mpool.Allocate<int>();
        *mem_1 = 42;
        dt::Console(*mem_1);
        dt::Console(&(*mem_1));

        int * mem_2 = mpool.Allocate<int>();
        *mem_2 = 43;
        dt::Console(*mem_2);
        dt::Console(mem_2);

        int * mem_3 = mpool.Allocate<int>();
        *mem_3 = 44;
        dt::Console(*mem_3);
        dt::Console(mem_3);

        int * mem_4 = mpool.Allocate<int>();
        *mem_4 = 45;
        dt::Console(*mem_4);
        dt::Console(mem_4);

        int * mem_5 = mpool.Allocate<int>();
        *mem_5 = 45;
        dt::Console(*mem_5);
        dt::Console(mem_5);

        int * mem_6 = mpool.Allocate<int>();
        if (mem_6 != nullptr) {
            *mem_6 = 46;
            dt::Console(*mem_6);
            dt::Console(mem_6);
        }

        dt::Console("Checking element deallocation");
        
        mpool.Deallocate(mem_1);
        mpool.Deallocate(mem_4);
        mpool.Deallocate(mem_5);
        mpool.Deallocate(mem_3);
        mpool.Deallocate(mem_2);

        dt::Console("Checking custom types");

        DummyClass * t_1 = mpool.Allocate<DummyClass>();
        t_1->Init();
        dt::Console(t_1->GetOne());
        dt::Console(t_1->GetTwo());
        t_1->PrintVector();
        DummyClass_2 * t_2 = mpool.Allocate<DummyClass_2>();
        if (t_2 != nullptr) {
            t_2->Init();
            dt::Console(t_2->GetOne());
            dt::Console(t_2->GetTwo());
            dt::Console(t_2->GetThree());
            dt::Console(t_2->GetFour());
            t_2->PrintVector();
            t_2->SetOne(33);
            dt::Console(t_2->GetOne());
        }

        DummyClass_2 * t_3 = mpool.Allocate<DummyClass_2>();
        t_3->Init();

        mpool.Deallocate(t_2);
        mpool.Deallocate(t_1);
        mpool.Deallocate(t_3);

    }

}

int main(int argc, char ** argv) {

    dt::Console(dt::INFO, "Checking Memory page...", dt::DARK_CYAN);
    CheckMemoryPage();
    dt::Console(dt::INFO, "Checking Array allocator...", dt::DARK_CYAN);
    CheckArrayAllocator();
    dt::Console(dt::INFO, "Checking Pool allocator...", dt::DARK_CYAN);
    CheckPoolAllocator();
    
#ifdef _WIN32
    system("pause");
#endif

}