#include <iostream>
#include <vector>

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

#include "game_engine/physics/Types.hpp"
#include "game_engine/physics/Geometry.hpp"
#include "game_engine/utility/QuadTree.hpp"
namespace ge = game_engine;

int main(int argc, char ** argv) {

    ms::PoolAllocator pool;
    dt::Console(pool.Init(sizeof(ge::QuadTree<int>), 2000000));
    
    ge::QuadTree<int> tree;
    tree.Init(ge::Rectangle2D_t(
        ge::Point2D_t(-200,-200), 
        ge::Point2D_t(200,-200), 
        ge::Point2D_t(200, 200), 
        ge::Point2D_t(-200, 200)),
        &pool
    );
    
    int data = 1;
    if (!tree.Insert({0,0}, &data)) dt::Console(dt::CRITICAL, "Insertion failed");    
    tree.PrettyPrint();

    dt::Console("---------------------------");
    
    int data_1 = 10;
    if (!tree.Insert({50,50}, &data_1)) dt::Console(dt::CRITICAL, "Insertion failed");    
    tree.PrettyPrint();

    dt::Console("---------------------------");
    
    int data_2 = 11;
    if (!tree.Insert({50,-50}, &data_2)) dt::Console(dt::CRITICAL, "Insertion failed");    
    tree.PrettyPrint();

    dt::Console("---------------------------");
    
    int data_3 = 12;
    if (!tree.Insert({-50,50}, &data_3)) dt::Console(dt::CRITICAL, "Insertion failed");    
    tree.PrettyPrint();

    dt::Console("---------------------------");
    
    int data_4 = 13;
    if (!tree.Insert({-50,-50}, &data_4)) dt::Console(dt::CRITICAL, "Insertion failed");    
    tree.PrettyPrint();

    dt::Console("---------------------------");
    
    int data_5 = 100;
    if (!tree.Insert({150, 150}, &data_5)) dt::Console(dt::CRITICAL, "Insertion failed");    
    tree.PrettyPrint();

    dt::Console("---------------------------");
    
    int data_6 = 1000;
    if (!tree.Insert({150, 150}, &data_6)) dt::Console(dt::CRITICAL, "Insertion failed");    
    tree.PrettyPrint();

    dt::Console("---------------------------");

    std::vector<int *> objects(15);
    size_t noo = tree.QueryRange(ge::Rectangle2D_t(
        ge::Point2D_t(0,0), 
        ge::Point2D_t(200,0), 
        ge::Point2D_t(200, 200), 
        ge::Point2D_t(0, 200)), objects);

    dt::Console("Elements: " + std::to_string(noo));
    for(size_t i =0; i<noo; i++) dt::Console(*objects.at(i));

    dt::Console("---------------------------");

    tree.Remove({150, 150}, &data_5);
    noo = tree.QueryRange(ge::Rectangle2D_t(
        ge::Point2D_t(0,0), 
        ge::Point2D_t(200,0), 
        ge::Point2D_t(200, 200), 
        ge::Point2D_t(0, 200)), objects);

    dt::Console("Elements: " + std::to_string(noo));
    for(size_t i =0; i<noo; i++) dt::Console(*objects.at(i));

    tree.Remove({150, 150}, &data_5);

    tree.Remove({150, 150}, &data_6);
    tree.Remove({0, 0}, &data);
    tree.Remove({50, 50}, &data_1);

    dt::Console("---------------------------");

    noo = tree.QueryRange(ge::Rectangle2D_t(
        ge::Point2D_t(-200,200), 
        ge::Point2D_t(200,-200), 
        ge::Point2D_t(200, 200), 
        ge::Point2D_t(-200, 200)), objects);

    dt::Console("Elements: " + std::to_string(noo));
    for(size_t i =0; i<noo; i++) dt::Console(*objects.at(i));

    tree.Remove({50, -50}, &data_2);
    tree.Remove({-50, 50}, &data_3);
    tree.Remove({-50, -50}, &data_4);

    noo = tree.QueryRange(ge::Rectangle2D_t(
        ge::Point2D_t(-200,200), 
        ge::Point2D_t(200,-200), 
        ge::Point2D_t(200, 200), 
        ge::Point2D_t(-200, 200)), objects);

    dt::Console("Elements: " + std::to_string(noo));
    for(size_t i =0; i<noo; i++) dt::Console(*objects.at(i));
    tree.PrettyPrint();

    dt::Console("---------------------------");

    tree.Insert({1,10}, new int(0));
    tree.Insert({30, 80}, new int (2));
    tree.Insert({-150, -180}, new int(3));
    tree.Insert({13, -56}, new int(7));
    tree.Insert({77,100}, new int(1));
    tree.Insert({85, -199}, new int(8));
    tree.Insert({-135, -2}, new int(4));
    tree.Insert({186, -175}, new int(9));
    tree.Insert({-10, 56}, new int (5));
    tree.Insert({-184, 56}, new int(6));

    noo = tree.QueryRange(ge::Rectangle2D_t(
        ge::Point2D_t(-200,200), 
        ge::Point2D_t(200,-200), 
        ge::Point2D_t(200, 200), 
        ge::Point2D_t(-200, 200)), objects);

    dt::Console("Elements: " + std::to_string(noo));
    for(size_t i =0; i<noo; i++) dt::Console(*objects.at(i));

    dt::Console("---------------------------");
    
    tree.PrettyPrint();
    tree.Destroy();
    
    dt::Console("---------------------------");

    tree.Init(ge::Rectangle2D_t(
        ge::Point2D_t(0, 0), 
        ge::Point2D_t(500000, 0), 
        ge::Point2D_t(500000, 500000), 
        ge::Point2D_t(0, 500000)),
        &pool);

    struct test_t{
        float x,y;
        int * d;
    };
    std::vector<test_t> a(1000000);
    for(size_t i=0; i<1000000; i++) {
        a[i].x = ge::GetRand(0, 500000);
        a[i].y = ge::GetRand(0, 500000);
        a[i].d = new int(i);
    }
    
    // dt::Console("Inserting...");
    // for(size_t i=0; i<1000000; i++) tree.Insert({a[i].x, a[i].y}, a[i].d);
    // dt::Console("Deleting...");
    // for(int i=999999; i>=0; i--) tree.Remove({a[i].x, a[i].y}, a[i].d);
    
    dt::Console("---------------------------");

    tree.Destroy();
    tree.Init(ge::Rectangle2D_t(
        ge::Point2D_t(0, 0), 
        ge::Point2D_t(200, 0), 
        ge::Point2D_t(200, 200), 
        ge::Point2D_t(0, 200)),
        &pool);
    tree.Insert({0,0}, &data);
    tree.Insert({0,1}, &data_1);
    tree.PrettyPrint();
    tree.Update({0,1}, &data_1, {155,0});
    tree.PrettyPrint();

    dt::Console("---------------------------");

    tree.Insert({156, 0}, &data_2);
    tree.PrettyPrint();
    tree.Update({156, 0}, &data_2, {155, 5});
    tree.PrettyPrint();


#ifdef _WIN32
    system("pause");
#endif

}