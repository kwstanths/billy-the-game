#ifdef _WIN32
#include <Windows.h>
#endif

#include <iostream>
#include <vector>

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

#include "game_engine/physics/Types.hpp"
#include "game_engine/physics/MathHelp.hpp"
#include "game_engine/utility/QuadTree.hpp"
namespace ge = game_engine;

int main(int argc, char ** argv) {

    ge::QuadTree<int> tree;
    tree.Init(ge::Rectangle2D_t(
        ge::Point2D_t(-200,-200), 
        ge::Point2D_t(200,-200), 
        ge::Point2D_t(200, 200), 
        ge::Point2D_t(-200, 200))
    );
    
    int data = 1;
    if (!tree.Insert({0,0}, &data)) dt::Console(dt::CRITICAL, "Insertion failed");    
    tree.Print();

    dt::Console("---------------------------");
    
    int data_1 = 10;
    if (!tree.Insert({50,50}, &data_1)) dt::Console(dt::CRITICAL, "Insertion failed");    
    tree.Print();

    dt::Console("---------------------------");
    
    int data_2 = 11;
    if (!tree.Insert({50,-50}, &data_2)) dt::Console(dt::CRITICAL, "Insertion failed");    
    tree.Print();

    dt::Console("---------------------------");
    
    int data_3 = 12;
    if (!tree.Insert({-50,50}, &data_3)) dt::Console(dt::CRITICAL, "Insertion failed");    
    tree.Print();

    dt::Console("---------------------------");
    
    int data_4 = 13;
    if (!tree.Insert({-50,-50}, &data_4)) dt::Console(dt::CRITICAL, "Insertion failed");    
    tree.Print();

    dt::Console("---------------------------");
    
    int data_5 = 100;
    if (!tree.Insert({150, 150}, &data_5)) dt::Console(dt::CRITICAL, "Insertion failed");    
    tree.Print();

    dt::Console("---------------------------");
    
    int data_6 = 1000;
    if (!tree.Insert({150, 150}, &data_6)) dt::Console(dt::CRITICAL, "Insertion failed");    
    tree.Print();

    dt::Console("---------------------------");

    std::vector<int *> objects(15);
    size_t noo = tree.QueryRange(ge::Rectangle2D_t(
        ge::Point2D_t(0,0), 
        ge::Point2D_t(200,0), 
        ge::Point2D_t(200, 200), 
        ge::Point2D_t(0, 200)), objects);

    dt::Console(noo);
    for(size_t i =0; i<noo; i++) dt::Console(*objects.at(i));


#ifdef _WIN32
    system("pause");
#endif

}