#ifndef __QuadTree_hpp__
#define __QuadTree_hpp__

#include <vector>

#include "game_engine/math/Types.hpp"
#include "game_engine/math/Geometry.hpp"

#include "game_engine/memory/PoolAllocator.hpp"

#include "debug_tools/Assert.hpp"
#include "debug_tools/Console.hpp"

namespace mth = game_engine::math;
namespace ms = game_engine::memory;
namespace dt = debug_tools;


namespace game_engine {
namespace utility {

    /* 
        A Point region quad tree to insert elements in points, remove items from points 
        and search for items in a region
    */
    template<typename T> class QuadTree {
    private:

        /* Internal struct to hold a saved element */
        struct QuadTreeData_t {
            /* Position in space */
            math::Point2D p_;
            /* The element */
            T data_;
            /* true, if it holds meaningfull data, false if not */
            bool holds_data_ = false;
        };

        /* iterator for the QuadTree */
        class QuadTreeIterator : public std::iterator<std::forward_iterator_tag, T> {
            friend class QuadTree;
        private:
            /* Holds the elements inside an area */
            std::vector<T *> elements_vector_;
            /* The index of the pointed element inside the elements_vector_ */
            size_t elements_vector_index_;
            T * pointed_;

            QuadTreeIterator(math::Rectangle2D brect, size_t n_elements, QuadTree * tree) {
                /* Initialize a vector with the elements */
                elements_vector_ = std::vector<T *>(n_elements, nullptr);
                elements_vector_index_ = 0;
                /* Get all elements inside the area */
                size_t elements_found = 0;
                tree->QueryRangePrivateDataPointers(brect, elements_vector_, elements_found);

                /* Set the pointed to point to the first element */
                pointed_ = elements_vector_[elements_vector_index_++];
            }

            QuadTreeIterator(T * pointed) : pointed_(pointed) {};

        public:
            T & operator*() {
                return *pointed_;
            }

            const QuadTreeIterator & operator++() {
                if (elements_vector_index_ >= elements_vector_.size()) pointed_ = nullptr;
                else pointed_ = elements_vector_[elements_vector_index_++];

                return *this;
            }

            const QuadTreeIterator& operator+=(size_t amount) {
                for (size_t i = 0; i < amount; i++)
                    operator++();

                return *this;
            }

            bool operator!=(const QuadTreeIterator& other) const {
                return this->pointed_ != other.pointed_;
            }

            bool operator==(const QuadTreeIterator& other) const {
                return this->pointed_ == other.pointed_;
            }

        };

        /* Objects needs to be initialised first */
        bool is_inited_;
        /* Holds the region that this quad tree node holds elements for */
        math::Rectangle2D brect_;
        /* Each quad tree node holds one element */
        QuadTreeData_t node_data_;
        /* Each quad tree node has four children */
        QuadTree<T> * child_nw_, * child_ne_, * child_sw_, * child_se_;
        
        /* The pool allocator to store the elements sequentially */
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

            Real_t x_west = brect_.A_.x_;
            Real_t x_middle = (brect_.A_.x_ + brect_.B_.x_)/2.0f;
            Real_t x_east = brect_.B_.x_;

            Real_t y_south = brect_.A_.y_;
            Real_t y_middle = (brect_.A_.y_ + brect_.D_.y_) / 2.0f;
            Real_t y_north = brect_.D_.y_;

            child_nw_->Init(math::Rectangle2D(
                math::Point2D(x_west, y_middle),
                math::Point2D(x_middle, y_middle),
                math::Point2D(x_middle, y_north),
                math::Point2D(x_west, y_north)),
                pool_);
            child_ne_->Init(math::Rectangle2D(
                math::Point2D(x_middle, y_middle),
                math::Point2D(x_east, y_middle),
                math::Point2D(x_east, y_north),
                math::Point2D(x_middle, y_north)),
                pool_);
            child_sw_->Init(math::Rectangle2D(
                math::Point2D(x_west, y_south),
                math::Point2D(x_middle, y_south),
                math::Point2D(x_middle, y_middle),
                math::Point2D(x_west, y_middle)),
                pool_);
            child_se_->Init(math::Rectangle2D(
                math::Point2D(x_middle, y_south),
                math::Point2D(x_east, y_south),
                math::Point2D(x_east, y_middle),
                math::Point2D(x_middle, y_middle)),
                pool_);
        } 

        /**
            Search for elements inside a region
            @param rect The region to search
            @param[out] objects The objects found
            @param[out] index The number of the last object found and stored inside objects
        */
        void QueryRangePrivate(math::Rectangle2D rect, std::vector<T>& objects, size_t& index){
            if (!mth::IntersectRect_Rect(brect_, rect)) return;

            /* Check this node's object */
            if (node_data_.holds_data_ && mth::PointInside(node_data_.p_, rect)) {
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

        void QueryRangePrivateDataPointers(math::Rectangle2D rect, std::vector<T*>& objects, size_t& index) {
            if (!mth::IntersectRect_Rect(brect_, rect)) return;

            /* Check this node's object */
            if (node_data_.holds_data_ && mth::PointInside(node_data_.p_, rect)) {
                if (index > objects.size() - 1) {
                    dt::Console(dt::CRITICAL, "QuadTree::QueryRange() Objects overflow");
                    return;
                }
                objects[index++] = &node_data_.data_;
            }

            /* Gather your children's objects */
            if (child_nw_ != nullptr) {
                child_nw_->QueryRangePrivateDataPointers(rect, objects, index);
                child_ne_->QueryRangePrivateDataPointers(rect, objects, index);
                child_sw_->QueryRangePrivateDataPointers(rect, objects, index);
                child_se_->QueryRangePrivateDataPointers(rect, objects, index);
            }
        }

        /**
            Update the position of an element from position p to new_p
            @param p The point position of the element
            @param data The element itself
            @param new_p The new position of the element
            @param root The root of the quad tree
            @return true = Updated, false = Element not found
        */
        bool UpdatePrivate(math::Point2D p, T data, math::Point2D new_p, QuadTree * root){
            if (!mth::PointInside(p, brect_)) return false;

            /* If this node has the point update here */
            if (node_data_.data_ == data && node_data_.p_ == p){
                /* If new position is within bounding rectangle dont move */
                if (mth::PointInside(new_p, brect_)) node_data_.p_ = new_p;
                else {
                    /* else remove it and insert from the beginning */
                    node_data_.holds_data_ = false;
                    root->Insert(new_p, data);
                }
                return true;
            }
            
            /* Recursively search in children */
            if(child_nw_ != nullptr){
                if (child_nw_->UpdatePrivate(p, data, new_p, root)) return true;
                if (child_ne_->UpdatePrivate(p, data, new_p, root)) return true;
                if (child_sw_->UpdatePrivate(p, data, new_p, root)) return true;
                if (child_se_->UpdatePrivate(p, data, new_p, root)) return true;
            }

            return false;
        }

    public:

        /*
            QuadTree iterator
        */
        typedef QuadTreeIterator iterator;

        /**
            Does nothing in particular. Call Init() to initialize
        */
        QuadTree() {
            is_inited_ = false;
            pool_ = nullptr;
        }

        /**
            Calls Destroy()
        */
        ~QuadTree() {
            Destroy();
        }

        static void * operator new(size_t size, ms::PoolAllocator * pool) {
            void * address = pool->Allocate<T>();
            if (address == nullptr) throw std::bad_alloc();
            return address;
        }

        static void operator delete(void * ptr, ms::PoolAllocator * pool) throw() {
            dt::Console(dt::FATAL, "QuadTree pool allocation failed");
        }

        /**
            Initialize the quad tree
            @param brect The bounding rectangle of the region of the quad tree
            @param pool The pool allocator to store elements
            @return true = OK, false = Already initialised
        */
        bool Init(math::Rectangle2D brect, ms::PoolAllocator * pool) {
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

            node_data_.holds_data_ = false;
            
            is_inited_ = false;

            return true;
        }

        void Clear() {
            if (!is_inited_) return;

            if (child_nw_ != nullptr) child_nw_->Clear();
            if (child_ne_ != nullptr) child_ne_->Clear();
            if (child_sw_ != nullptr) child_sw_->Clear();
            if (child_se_ != nullptr) child_se_->Clear();

            node_data_.holds_data_ = false;
        }

        /**
            Insert an element inside the quad tree
            @param p The point of the element
            @param data The actual data
            @return true = OK, false = Out of region
        */
        bool Insert(math::Point2D p, T data) {
            if (!is_inited_) return false;
            
            /* If point to be inserted is not inside the current quad tree rectangle, return */
            if (!mth::PointInside(p, brect_)) return false;
            
            /* If the current node does not hold anything, insert here */
            if (!node_data_.holds_data_) {
                node_data_.p_ = p;
                node_data_.data_ = data;
                node_data_.holds_data_ = true;
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
            dt::ConsoleInfoL(dt::CRITICAL, "QuadTree::Insert(): Reached, unreachable point. Cannot insert into quad tree",
                "point", p);

            return false;
        }

        /**
            Get the elements inside a rectangle region
            @param rect The region
            @param[out] objects The elements
            @return The number of elements found
        */
        size_t QueryRange(math::Rectangle2D rect, std::vector<T>& objects){
            if (!is_inited_) return 0;

            size_t number_of_objects = 0;
            QueryRangePrivate(rect, objects, number_of_objects);

            return number_of_objects;
        }

        /**
            Update the position of element data from position p to new_p
            @param p The point position of the element
            @param data The element itself
            @param new_p The new position of the element
            @return true = Updated, false = Element not found            
        */
        bool Update(math::Point2D p, T data, math::Point2D new_p){
            if (!is_inited_) return 0;

            return UpdatePrivate(p, data, new_p, this);
        }

        /**
            Remove an element from the tree
            @param p The point of the element
            @param data The element itself
            @return true = OK, false = Not found
        */
        bool Remove(math::Point2D p, T data){
            if (!is_inited_) return false;

            /* If point to be removed is not inside the current quad tree rectangle, return */
            if (!mth::PointInside(p, brect_)) return false;

            /* If the current node has the element, remove it from here */
            if (node_data_.data_ == data && node_data_.p_ == p) {
                node_data_.holds_data_ = false;
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

            for (size_t i = 0; i < level; i++) dt::CustomPrint(std::cout, "-");
            if (node_data_.holds_data_) {
                std::cout << "(" << std::to_string(node_data_.p_.x_) << "," << std::to_string(node_data_.p_.y_)  << "): " 
                    << node_data_.data_ << std::endl;
            } else dt::Console("no data");

            if (child_nw_ != nullptr){
                child_nw_->PrettyPrint(level+1);
                child_ne_->PrettyPrint(level+1);
                child_sw_->PrettyPrint(level+1);
                child_se_->PrettyPrint(level+1);
            }
        }

        /*
            Get an iterator to the first element in a region
            @param brect The area to search
            @param n_elements The max number of elements to get
        */
        iterator begin(math::Rectangle2D brect, size_t n_elements) {
            return QuadTreeIterator(brect, n_elements, this);
        }


        /*
            Get an iterator to the end
        */
        iterator end() {
            return QuadTreeIterator(nullptr);
        }
    };

}
}

#endif
