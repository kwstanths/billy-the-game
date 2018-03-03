#ifdef _WIN32
#include <Windows.h>
#endif

#include <iostream>
#include <vector>

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

#include "game_engine/utility/QuadTree.hpp"
namespace ge = game_engine;

int main(int argc, char ** argv) {

    ge::QuadTree<int> tree;



    
#ifdef _WIN32
    system("pause");
#endif

}