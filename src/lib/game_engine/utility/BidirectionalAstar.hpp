#ifndef __BidirectionalAstar_hpp__
#define __BidirectionalAstar_hpp__

#include <queue>
#include <functional>
#include <vector>
#include <unordered_map>

#include "game_engine/utility/UniformGrid.hpp"
#include "game_engine/math/Real.hpp"

namespace game_engine {
namespace utility {

    class BidirectionalAstar {
    public:
        struct CELL {
            int i_, j_;
            CELL() : i_(0), j_(0) {};
            CELL(int i, int j) : i_(i), j_(j) {};

            bool operator==(const CELL& other) const {
                return i_ == other.i_ && j_ == other.j_;
            }
            bool operator!=(const CELL& other) const {
                return i_ != other.i_ || j_ != other.j_;
            }
            CELL operator+(const CELL& other) const {
                return CELL(i_ + other.i_, j_ + other.j_);
            }
        };

        BidirectionalAstar();

        void Init(game_engine::utility::UniformGrid<int, 2> * grid, CELL start, CELL goal);

        /* Run a single step of the forward pass */
        bool StepF();
        /* Run a single step of the backwards pass */
        bool StepBI();

        /* Run the full forward search algorithm */
        bool FullRunF();
        /* Run the full bidirectional search algorithm */
        bool FullRunBI();

        /* Get the found path, make sure that one of the step or FullRun calls returned true */
        std::vector<CELL> GetPath();

    private:
        /* Holds the grid structure */
        game_engine::utility::UniformGrid<int, 2> * grid_ = nullptr;
        /* Priority queues for the A* star algorithms, element to be picked the one with the smallest G + H */
        std::priority_queue<CELL, std::vector<CELL>, std::function<bool(CELL, CELL)>> open_set_forward_, open_set_backwards_;
        /* Hold where we came from, the currently best known */
        std::unordered_map<size_t, CELL> came_from_forward_, came_from_backwards_;
        /* Hold the current known best cost to go to that node */
        std::vector<game_engine::Real_t> g_forward_, g_backwards_;
        CELL start_, goal_, current_forward_, current_backwards_, meeting_cell_;
        size_t nodes_visited_ = 0;
        std::vector<std::pair<CELL, game_engine::Real_t>> neighbors_;

        bool StepForward();
        bool StepBackwards();

        bool comparatorF(CELL lhs, CELL rhs);
        bool comparatorB(CELL lhs, CELL rhs);

        game_engine::Real_t heuristicF(CELL cell);
        game_engine::Real_t heuristicB(CELL cell);

        size_t GetCellIndex(CELL& cell);

        std::vector<CELL> ReconstructPath();
    };

}
}

#endif