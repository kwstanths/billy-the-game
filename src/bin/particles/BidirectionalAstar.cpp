#include "BidirectionalAstar.hpp"

#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

namespace ge = game_engine;

BidirectionalAstar::BidirectionalAstar() {

}

void BidirectionalAstar::Init(game_engine::utility::UniformGrid<int, 2>* grid, CELL start, CELL goal) {
    grid_ = grid;
    start_ = start;
    goal_ = goal;

    /* Initialize */
    nodes_visited_ = 0;

    current_forward_ = start_;
    current_backwards_ = goal_;

    open_set_forward_ = std::priority_queue<CELL, std::vector<CELL>, std::function<bool(CELL, CELL)> >(std::bind(&BidirectionalAstar::comparatorF, this, std::placeholders::_1, std::placeholders::_2));
    open_set_forward_.push(start_);
    open_set_backwards_ = std::priority_queue<CELL, std::vector<CELL>, std::function<bool(CELL, CELL)> >(std::bind(&BidirectionalAstar::comparatorB, this, std::placeholders::_1, std::placeholders::_2));
    open_set_backwards_.push(goal_);

    came_from_forward_.clear();
    came_from_backwards_.clear();

    std::vector<size_t> dimensions = grid_->GetDimensions();
    g_forward_ = std::vector<game_engine::Real_t>(dimensions[0] * dimensions[1], std::numeric_limits<game_engine::Real_t>::infinity());
    g_forward_[GetCellIndex(start_)] = 0;
    g_backwards_ = std::vector<game_engine::Real_t>(dimensions[0] * dimensions[1], std::numeric_limits<game_engine::Real_t>::infinity());
    g_backwards_[GetCellIndex(goal_)] = 0;

    ge::Real_t sqrt2 = std::sqrt(2);
    neighbors_ = std::vector<std::pair<CELL, game_engine::Real_t>>(8);
    neighbors_.push_back(std::make_pair(CELL(0, 1), 1));
    neighbors_.push_back(std::make_pair(CELL(0, -1), 1));
    neighbors_.push_back(std::make_pair(CELL(1, 0), 1));
    neighbors_.push_back(std::make_pair(CELL(-1, 0), 1));
    neighbors_.push_back(std::make_pair(CELL(1, 1), sqrt2));
    neighbors_.push_back(std::make_pair(CELL(-1, 1), sqrt2));
    neighbors_.push_back(std::make_pair(CELL(1, -1), sqrt2));
    neighbors_.push_back(std::make_pair(CELL(-1, -1), sqrt2));
}

bool BidirectionalAstar::StepF() {
    bool reached = StepForward();
    return reached;
}

bool BidirectionalAstar::StepBI() {
    bool reached = StepForward();
    reached = reached || StepBackwards();
    return reached;
}

bool BidirectionalAstar::FullRunBI() {
    bool reached = false;
    while (!reached && !open_set_forward_.empty() && !open_set_backwards_.empty()) {
        reached = StepBI();
    }

    if (!reached) {
        dt::ConsoleInfoL(dt::WARNING, "Failed to find path...", "start i", start_.i_, "start j", start_.j_, "goal i", goal_.i_, "goal j", goal_.j_);
    }
    return reached;
}

bool BidirectionalAstar::FullRunF() {
    bool reached = false;
    while (!reached && !open_set_forward_.empty()) {
        reached = StepF();
    }

    if (!reached) {
        dt::ConsoleInfoL(dt::WARNING, "Failed to find path...", "start i", start_.i_, "start j", start_.j_, "goal i", goal_.i_, "goal j", goal_.j_);
    }
    return reached;
}

std::vector<BidirectionalAstar::CELL> BidirectionalAstar::GetPath() {
    return ReconstructPath();
}

game_engine::Real_t BidirectionalAstar::heuristicF(CELL cell) {
    return /*1.001**/(g_forward_[GetCellIndex(cell)] + std::abs((int)cell.i_ - (int)goal_.i_) + std::abs((int)cell.j_ - (int)goal_.j_));
}

game_engine::Real_t BidirectionalAstar::heuristicB(CELL cell) {
    return /*1.001**/(g_backwards_[GetCellIndex(cell)] + std::abs((int)cell.i_ - (int)start_.i_) + std::abs((int)cell.j_ - (int)start_.j_));
}

bool BidirectionalAstar::StepForward() {
    current_forward_ = open_set_forward_.top();
    open_set_forward_.pop();
    nodes_visited_++;

    /* If that node is already visited by the other search, stop */
    if (g_backwards_[GetCellIndex(current_forward_)] != std::numeric_limits<game_engine::Real_t>::infinity()) {
        meeting_cell_ = current_forward_;
        return true;
    }

    for (size_t n = 0; n < neighbors_.size(); n++) {
        CELL neighbor = current_forward_ + neighbors_[n].first;
        size_t neighbor_index = GetCellIndex(neighbor);
        if (grid_->at(neighbor.i_, neighbor.j_) == -1) continue;

        ge::Real_t tentative_score = g_forward_[GetCellIndex(current_forward_)] + neighbors_[n].second;
        /* If this new cost is better for that node */
        if (tentative_score < g_forward_[neighbor_index]) {
            /* Store how we got here */
            came_from_forward_[neighbor_index] = current_forward_;
            g_forward_[neighbor_index] = tentative_score;
            /* If node not already visited, add to the queue */
            if (g_forward_[neighbor_index] != std::numeric_limits<ge::Real_t>::infinity()) open_set_forward_.push(neighbor);
        }
    }

    return false;
}

bool BidirectionalAstar::StepBackwards() {
    current_backwards_ = open_set_backwards_.top();
    open_set_backwards_.pop();
    nodes_visited_++;

    /* If that node is already visited by the other search, stop */
    if (g_forward_[GetCellIndex(current_backwards_)] != std::numeric_limits<game_engine::Real_t>::infinity()) {
        meeting_cell_ = current_backwards_;
        return true;
    }

    for (size_t n = 0; n < neighbors_.size(); n++) {
        CELL neighbor = current_backwards_ + neighbors_[n].first;
        size_t neighbor_index = GetCellIndex(neighbor);
        if (grid_->at(neighbor.i_, neighbor.j_) == -1) continue;

        ge::Real_t tentative_score = g_backwards_[GetCellIndex(current_backwards_)] + neighbors_[n].second;
        /* If this new cost is better for that node */
        if (tentative_score < g_backwards_[neighbor_index]) {
            /* Store how we got here */
            came_from_backwards_[neighbor_index] = current_backwards_;
            g_backwards_[neighbor_index] = tentative_score;
            /* If node not already visited, add to the queue */
            if (g_backwards_[neighbor_index] != std::numeric_limits<ge::Real_t>::infinity()) open_set_backwards_.push(neighbor);
        }
    }

    return false;
}

bool BidirectionalAstar::comparatorF(CELL lhs, CELL rhs) {
    return heuristicF(lhs) > heuristicF(rhs);
}

bool BidirectionalAstar::comparatorB(CELL lhs, CELL rhs) {
    return heuristicB(lhs) > heuristicB(rhs);
}

size_t BidirectionalAstar::GetCellIndex(CELL & cell) {
    std::vector<size_t> dimensions = grid_->GetDimensions();
    return cell.i_ * dimensions[1] + cell.j_;
}

std::vector<BidirectionalAstar::CELL> BidirectionalAstar::ReconstructPath() {
    CELL current = meeting_cell_;

    /* Append forward path */
    std::vector<CELL> cells;
    cells.push_back(current);

    while (!(current == start_)) {
        current = came_from_forward_[GetCellIndex(current)];
        cells.push_back(current);
    }
    std::reverse(cells.begin(), cells.end());

    /* Append backwards path */
    current = meeting_cell_;
    while (!(current == goal_)) {
        current = came_from_backwards_[GetCellIndex(current)];
        cells.push_back(current);
    }

    return cells;
}
