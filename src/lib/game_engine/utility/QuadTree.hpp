#ifndef __QuadTree_hpp__
#define __QuadTree_hpp__

#include "game_engine/physics/Types.hpp"

#include "game_engine/memory/PoolAllocator.hpp"
namespace ms = memory_subsystem;


namespace game_engine {

    template<typename T> class QuadTree {
    private:
        
        const size_t NODE_CAPACITY = 4;

        typedef struct QuadTreeData {
            Point2D_t p_;
            T * data_;
            struct QuadTreeData(Point2D_t p, T * data) : p_(p), data_(data) {};
        } QuadTreeData_t;
        
    private:
        bool is_inited_;
        Rectangle2D_t brect_;
        QuadTree * child_nw_, child_ne_, child_sw_, child_se_;
        
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

            node_ = nullptr;

            is_inited_ = false;
            return true;
        }

        bool Insert(Point2D_t p, T * data) {
            
        }

        
    };

}

#endif