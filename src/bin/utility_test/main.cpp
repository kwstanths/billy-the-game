#include <iostream>
#include <vector>

#include "game_engine/math/RNGenerator.hpp"
#include "game_engine/utility/QuadTree.hpp"
#include "game_engine/utility/QuadTreeBoxes.hpp"
#include "game_engine/math/Types.hpp"
#include "game_engine/math/AABox.hpp"
#include "game_engine/utility/List.hpp"
#include "game_engine/utility/HashTable.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;
namespace ge = game_engine;
namespace utl = game_engine::utility;

using namespace ge;

int main(int argc, char ** argv) {

    utl::QuadTree<int> tree(Point2D({ -100, -100 }), 200);

    int data_1 = 3;
    if (!tree.Insert(Point2D({50,50}), data_1)) dt::Console(dt::CRITICAL, "Insertion failed");    

    int data_2 = 2;
    if (!tree.Insert(Point2D({50,-50}), data_2)) dt::Console(dt::CRITICAL, "Insertion failed");    

    int data_3 = 1;
    if (!tree.Insert(Point2D({-50,50}), data_3)) dt::Console(dt::CRITICAL, "Insertion failed");

    int data_4 = 0;
    if (!tree.Insert(Point2D({-50,-50}), data_4)) dt::Console(dt::CRITICAL, "Insertion failed");

    dt::Console("---------------------------");
    std::vector<int> objects;

    tree.QueryRange(math::AABox<2>(
        Point2D({ -70, -70 }),
        Point2D({ -10, 70})), objects);

    dt::Console("---------------------------");
    objects.clear();

    float angle = 270;
    tree.RayCast(Ray2D(Point2D({ -50, 50}), angle), objects);

    dt::Console("---------------------------");
    objects.clear();

    {
        utl::QuadTreeBoxes<int, 1> tree(math::Point2D(-50), 100);
        tree.Insert(1, AABox<2>(math::Point2D({ 10, 10 }), { 10, 10 }));
        tree.Insert(3, AABox<2>(math::Point2D({ -15, -15 }), { 15, 15 }));
        tree.Insert(4, AABox<2>(math::Point2D({ 25, -25 }), { 15, 5 }));
        tree.Insert(5, AABox<2>(math::Point2D({ -20, 20 }), { 5, 15 }));
        //tree.Insert(6, AABox<2>(math::Point2D({ 0, 0 }), { 5, 5 }));
        
        dt::Console("---------------------------");
        std::vector<int> objects;


        AABox<2> box(Point2D({ -2, -2 }), { 2,2 });
        float angle = 180 + 45;
        Ray2D ray(Point2D({ 0,0 }), angle);
        Real_t t;
        bool ret = IntersectionAABoxRay2D(box, ray, t);
        dt::Console("---------------------------");

        angle = 90;
        tree.RayCast(Ray2D(Point2D({ 1, 10}), angle), objects);
        dt::Console("---------------------------");

    }

#ifdef _WIN32
    system("pause");
#endif

}