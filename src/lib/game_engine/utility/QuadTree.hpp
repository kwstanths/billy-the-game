#ifndef __QuadTree_hpp__
#define __QuadTree_hpp__

#include <vector>

#include "game_engine/physics/Types.hpp"
#include "game_engine/physics/MathHelp.hpp"

#include "game_engine/memory/PoolAllocator.hpp"
namespace ms = memory_subsystem;


namespace game_engine {

    template<typename T> class QuadTree {

        const size_t NODE_CAPACITY = 4;

    private:
        bool is_inited_;
        Rectangle2D_t brect_;
        std::vector<T *> data_;
        QuadTree<T> * child_nw_, * child_ne_, * child_sw_, * child_se_;
        
        void Subdivide(){
            child_nw_ = new QuadTree<T>();
            child_ne_ = new QuadTree<T>();
            child_se_ = new QuadTree<T>();
            child_sw_ = new QuadTree<T>();

            float x_west = brect_.A_.x_;
            float x_middle = (brect_.A_.x_ + brect_.B_.x_)/2.0f;
            float x_east = brect_.B_.x_;

            float y_south = brect_.A_.y_;
            float y_middle = (brect_.A_.y_ + brect_.D_.y_) / 2.0f;
            float y_north = brect_.A_.y_;

            child_nw_->Init(Rectangle2D_t(
                Point2D_t(x_west, y_middle),
                Point2D_t(x_middle, y_middle),
                Point2D_t(x_middle, y_north),
                Point2D_t(x_west, y_north)));
            child_ne_->Init(Rectangle2D_t(
                Point2D_t(x_middle, y_middle),
                Point2D_t(x_east, y_middle),
                Point2D_t(x_east, y_north),
                Point2D_t(x_middle, y_north)));
            child_sw_->Init(Rectangle2D_t(
                Point2D_t(x_west, y_south),
                Point2D_t(x_middle, y_south),
                Point2D_t(x_middle, y_middle),
                Point2D_t(x_west, y_middle)));
            child_se_->Init(Rectangle2D_t(
                Point2D_t(x_middle, y_south),
                Point2D_t(x_east, y_south),
                Point2D_t(x_east, y_middle),
                Point2D_t(x_middle, y_middle)));
        } 

    public:
        QuadTree() {
            is_inited_ = false;
        }
        
        ~QuadTree() {
            Destroy();
        }

        bool Init(Rectangle2D_t brect) {
            if (is_inited_) return false;

            brect = brect_;
            child_nw_ = nullptr;
            child_ne_ = nullptr;
            child_sw_ = nullptr;
            child_se_ = nullptr;
            
            is_inited_ = true;
            return true;
        }

        bool Destroy() {
            if (!is_inited_) return false;

            if (child_nw_ != nullptr){
                child_nw_->Destroy();
                delete child_nw_;
                child_nw_ = nullptr;
            }
            if (child_ne_ != nullptr){
                child_ne_->Destroy();
                delete child_ne_;
                child_ne_ = nullptr;
            }
            if (child_sw_ != nullptr){
                child_sw_->Destroy();
                delete child_sw_;
                child_sw_ = nullptr;
            }
            if (child_se_ != nullptr){
                child_se_->Destroy();
                delete child_se_;
                child_se_ = nullptr;
            }
            data_.clear();

            is_inited_ = false;
            return true;
        }

        bool Insert(Point2D_t p, T * data) {
            /* If point to be inserted is not inside the current quad tree rectangle, return false */
            if (!PointInside(p, brect_)) return false;

            if (data_.size() < NODE_CAPACITY) data_.push_back(data);



        }

        
    };

}

#endif