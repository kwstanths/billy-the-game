#ifndef __QuadTree_hpp__
#define __QuadTree_hpp__

#include <vector>

#include "game_engine/physics/Types.hpp"
#include "game_engine/physics/Geometry.hpp"

#include "game_engine/memory/PoolAllocator.hpp"
namespace ms = memory_subsystem;

#include "debug_tools/Assert.hpp"
#include "debug_tools/Console.hpp"
namespace dt = debug_tools;

namespace game_engine {

    /* 
        A Point region quad tree to insert elements in points, remove items from points 
        and search for items in a region
    */
    template<typename T> class QuadTree {
    private:
        /* Internal struct to hold a saved element */
        struct QuadTreeData_t {
            /* Position in space */
            Point2D_t p_;
            /* Pointer to element */
            T * data_;
        };

        /* Objects needs to be initialised first */
        bool is_inited_;
        /* Holds the region that this quad tree node holds elements for */
        Rectangle2D_t brect_;
        /* Each quad tree node holds on element */
        QuadTreeData_t node_data_;
        /* Each quad tree node has four children */
        QuadTree<T> * child_nw_, * child_ne_, * child_sw_, * child_se_;
        
        /* The pool allocator to store the elements */
        ms::PoolAllocator * pool_;

        /**
            Divide the area of this node into four regions and initialize 
            the four children nodes
        */
        void Subdivide(){
            child_nw_ = new (pool_) QuadTree<T>();
            child_ne_ = new (pool_) QuadTree<T>();
            child_se_ = new (pool_) QuadTree<T>();
            child_sw_ = new (pool_) QuadTree<T>();

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
                Point2D_t(x_west, y_north)),
                pool_);
            child_ne_->Init(Rectangle2D_t(
                Point2D_t(x_middle, y_middle),
                Point2D_t(x_east, y_middle),
                Point2D_t(x_east, y_north),
                Point2D_t(x_middle, y_north)),
                pool_);
            child_sw_->Init(Rectangle2D_t(
                Point2D_t(x_west, y_south),
                Point2D_t(x_middle, y_south),
                Point2D_t(x_middle, y_middle),
                Point2D_t(x_west, y_middle)),
                pool_);
            child_se_->Init(Rectangle2D_t(
                Point2D_t(x_middle, y_south),
                Point2D_t(x_east, y_south),
                Point2D_t(x_east, y_middle),
                Point2D_t(x_middle, y_middle)),
                pool_);
        } 

        /**
            Search for elements inside a region
            @param rect The region to search
            @param[out] objects The objects found
            @param[out] index The number of the last object found and stored inside objects
        */
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
        /**
            Does nothing in particular. Call Init() to initialize
        */
        QuadTree() {
            is_inited_ = false;
            node_data_.data_ = nullptr;
            pool_ = nullptr;
        }

        /**
            Calls Destroy()
        */
        ~QuadTree() {
            Destroy();
        }

        static void * operator new(size_t size, ms::PoolAllocator * pool) throw() {
            void * address = pool->Allocate<T>();
            if (address == nullptr) throw std::bad_alloc();
            return address;
        }

        static void operator delete(void * ptr, ms::PoolAllocator * pool) throw() {
            dt::Console(dt::FATAL, "QuadTree initialization failed");
        }

        /**
            Initialize the quad tree
            @param brect The bounding rectangle of the region of the quad tree
            @param pool The pool allocator to store elements
            @return true = OK, false = Already initialised
        */
        bool Init(Rectangle2D_t brect, ms::PoolAllocator * pool) {
            if (is_inited_) return false;

            brect_ = brect;
            child_nw_ = nullptr;
            child_ne_ = nullptr;
            child_sw_ = nullptr;
            child_se_ = nullptr;
            pool_ = pool;
            if (!pool_->IsInited()) return false;
            
            is_inited_ = true;
            return true;
        }

        /**
            Destroy the object, Needs init to be called again. Does not deallocate the
            actual data held
            @return true = OK, false = Already destroyed, or not initialised
        */
        bool Destroy() {
            if (!is_inited_) return false;

            if (child_nw_ != nullptr){
                child_nw_->Destroy();
                pool_->Deallocate(child_nw_);
                child_nw_ = nullptr;
            }
            if (child_ne_ != nullptr){
                child_ne_->Destroy();
                pool_->Deallocate(child_ne_);
                child_ne_ = nullptr;
            }
            if (child_sw_ != nullptr){
                child_sw_->Destroy();
                pool_->Deallocate(child_sw_);
                child_sw_ = nullptr;
            }
            if (child_se_ != nullptr){
                child_se_->Destroy();
                pool_->Deallocate(child_se_);
                child_se_ = nullptr;
            }

            node_data_.data_ = nullptr;
            
            is_inited_ = false;

            return true;
        }

        /**
            Insert an element inside the quad tree
            @param p The point of the element
            @param data The actual data
            @return true = OK, false = Out of region
        */
        bool Insert(Point2D_t p, T * data) {
            if (!is_inited_) return false;

            _assert(data != nullptr);
            
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

        /**
            Get the elements inside a rectangle region
            @param rect The region
            @param[out] objects The elements
            @return The number of elements found
        */
        size_t QueryRange(Rectangle2D_t rect, std::vector<T *>& objects){
            if (!is_inited_) return 0;

            size_t number_of_objects = 0;
            QueryRangePrivate(rect, objects, number_of_objects);

            return number_of_objects;
        }

        /**
            Remove an element from the tree
            @param p The point of the element
            @param data The element itself
            @return true = OK, false = Not found
        */
        bool Remove(Point2D_t p, T * data){
            if (!is_inited_) return false;

            _assert(data != nullptr);

            /* If point to be removed is not inside the current quad tree rectangle, return */
            if (!PointInside(p, brect_)) return false;

            /* If the current node has the element, remove it from here */
            if (node_data_.data_ == data && node_data_.p_ == p) {
                node_data_.data_ = nullptr;
                return true;
            }
            
            /* Else search the children */
            if (child_nw_ != nullptr){
                if (child_nw_->Remove(p, data)) return true;
                if (child_ne_->Remove(p, data)) return true;
                if (child_sw_->Remove(p, data)) return true;
                if (child_se_->Remove(p, data)) return true;
            }

            return false;
        }

        /**
            Prints a "pretty" layout of the elements inside the tree
            @param level The level of the starting node
        */
        void PrettyPrint(size_t level = 0){
            if (!is_inited_) return;

            for(size_t i=0; i<level; i++) dt::CustomPrint(std::cout, "-");
            if (node_data_.data_ != nullptr) dt::Console(*node_data_.data_ );
            else dt::Console("null");

            if (child_nw_ != nullptr){
                child_nw_->PrettyPrint(level+1);
                child_ne_->PrettyPrint(level+1);
                child_sw_->PrettyPrint(level+1);
                child_se_->PrettyPrint(level+1);
            }
        }
    };

}

#endif