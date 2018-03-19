#ifndef __QuadTree_hpp__
#define __QuadTree_hpp__

#include <vector>

#include "game_engine/physics/Types.hpp"
#include "game_engine/physics/MathHelp.hpp"

#include "game_engine/memory/PoolAllocator.hpp"
namespace ms = memory_subsystem;

#include "debug_tools/Assert.hpp"
#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

namespace game_engine {

    template<typename T> class QuadTree {
    private:

        struct QuadTreeData_t {
            Point2D_t p_;
            T * data_;
        };

        bool is_inited_;
        Rectangle2D_t brect_;
        QuadTreeData_t node_data_;
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
            float y_north = brect_.D_.y_;

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

        void QueryRangePrivate(Rectangle2D_t rect, std::vector<T *>& objects, size_t& index){
            if (!IntersectRect_Rect(brect_, rect)) return;

            /* Check this node's object */
            if (node_data_.data_ != nullptr && PointInside(node_data_.p_, rect)) {
                if (index > objects.size() - 1) {
                    dt::Console(dt::CRITICAL, "QuadTree::QueryRange() Objects overflow");
                    return;
                }
                objects[index++] = node_data_.data_;            
            }

            /* Gather your children's objects */
            if (child_nw_ != nullptr){
                child_nw_->QueryRangePrivate(rect, objects, index);
                child_ne_->QueryRangePrivate(rect, objects, index);
                child_sw_->QueryRangePrivate(rect, objects, index);
                child_se_->QueryRangePrivate(rect, objects, index);
            }
        }

    public:
        QuadTree() {
            is_inited_ = false;
            node_data_.data_ = nullptr;
        }
        
        ~QuadTree() {
            Destroy();
        }

        bool Init(Rectangle2D_t brect) {
            if (is_inited_) return false;

            brect_ = brect;
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
            
            is_inited_ = false;
            return true;
        }

        bool Insert(Point2D_t p, T * data) {
            if (!is_inited_) return false;

            /* If point to be inserted is not inside the current quad tree rectangle, return */
            if (!PointInside(p, brect_)) return false;

            /* If the current node does not hold anything, insert here */
            if (node_data_.data_ == nullptr) {
                node_data_.p_ = p;
                node_data_.data_ = data;
                return true;
            }

            /* Create four children if we dont already have them */
            if (child_nw_ == nullptr) Subdivide();

            /* Insert to children */
            if (child_nw_->Insert(p, data)) return true;
            if (child_ne_->Insert(p, data)) return true;
            if (child_sw_->Insert(p, data)) return true;
            if (child_se_->Insert(p, data)) return true;

            /* We should never reach this point */
            _assert(0);
        }

        size_t QueryRange(Rectangle2D_t rect, std::vector<T *>& objects){
            if (!is_inited_) return 0;

            size_t number_of_objects = 0;
            QueryRangePrivate(rect, objects, number_of_objects);

            return number_of_objects;
        }

        void Print(){
            if (!is_inited_) return;

            if (node_data_.data_ != nullptr) dt::Console(*node_data_.data_);
            else dt::Console("null");

            if (child_nw_ != nullptr){
                child_nw_->Print();
                child_ne_->Print();
                child_sw_->Print();
                child_se_->Print();
            }
        }
    };

}

#endif