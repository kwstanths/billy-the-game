#ifndef __QuadTree_hpp__
#define __QuadTree_hpp__

#include <vector>
#include <deque>
#include <algorithm>

#include "game_engine/math/Types.hpp"
#include "game_engine/math/AABox.hpp"
#include "game_engine/math/Geometry.hpp"

#include "game_engine/memory/PoolAllocator.hpp"

#include "debug_tools/Assert.hpp"
#include "debug_tools/Console.hpp"

namespace mth = game_engine::math;
namespace ms = game_engine::memory;
namespace dt = debug_tools;

using namespace game_engine::math;

namespace game_engine {
namespace utility {

    template<typename Data, int BUCKET_SIZE = 1, int MAX_DEPTH = 13>
    class QuadTree {
    private:
        class QuadTreeNode {
        public:
            enum NodeType {
                LEAF,
                INNER,
            };

            virtual ~QuadTreeNode() {

            }

            NodeType GetNodeType() { return type_; }

            AABox<2> GetAABox() {
                return AABox<2>(origin_, origin_ + Point2D(length_));
            }

            virtual void Destroy() = 0;
            virtual QuadTreeNode * Insert(Point2D point, Data data, size_t depth) = 0;
            virtual size_t QueryRange(AABox<2> search_box, std::vector<Data>& results) = 0;
            virtual QuadTreeNode * Remove(Point2D point) = 0;
            virtual size_t Depth() = 0;

            virtual void RayCastProcessChild(Real_t tx0, Real_t ty0, Real_t tx1, Real_t ty1, unsigned char a, std::vector<Data>& results) = 0;

        protected:
            NodeType type_;
            Point2D origin_;
            Real_t length_;

            /**
               Find the index of the child, and the new orign
            */
            std::pair<Point2D, size_t> FindChild(Point2D& point) {
                /**
                    Find in which quadrant this point lies to, and what's the origin
                    point of this quadrant. Return both
                */
                size_t index = 0;
                Real_t H = length_ / 2.0f;
                Point2D new_node_origin = origin_;

                Point2D center = origin_ + Point2D(H);
                Point2D V = point - center;

                if (V[0] >= 0) {
                    index += 2;
                    new_node_origin[0] += H;
                }
                if (V[1] >= 0) {
                    index += 1;
                    new_node_origin[1] += H;
                }

                return std::make_pair(new_node_origin, index);
            }

        };

        class QuadTreeLeafNode : public QuadTreeNode {
        public:
            QuadTreeLeafNode(Point2D origin, Real_t length) {
                this->origin_ = origin;
                this->length_ = length;
                this->type_ = QuadTreeNode::NodeType::LEAF;
            }

            void Destroy() {

            }

            QuadTreeNode * Insert(Point2D point, Data data, size_t depth) {
                /* If this leaf has enough space or maximum depth reached, store here */
                if (buckets_.size() < BUCKET_SIZE || depth >= MAX_DEPTH) {
                    buckets_.push_back(Bucket(point, data));
                    return this;
                }

                /* Either-wise, split the leaf */
                QuadTreeInnerNode * temp = new  QuadTreeInnerNode(this->origin_, this->length_);
                for (size_t i = 0; i < buckets_.size(); i++) {
                    temp->Insert(buckets_[i].point_, buckets_[i].data_, depth + 1);
                }
                temp->Insert(point, data, depth + 1);

                delete this;
                return temp;
            }

            size_t QueryRange(AABox<2> search_box, std::vector<Data>& results) {

                size_t points = 0;
                for (size_t i = 0; i < buckets_.size(); i++) {
                    if (search_box.Inside(buckets_[i].point_)) {
                        results.push_back(buckets_[i].data_);
                        points++;
                    }
                }

                return points;
            }

            QuadTreeLeafNode * Remove(Point2D point) {
                /* Check if the point is stored here, and delete it */
                for (typename std::deque<Bucket>::iterator itr = buckets_.begin(); itr != buckets_.end(); ++itr) {
                    if ((*itr).point_ == point) {
                        buckets_.erase(itr);
                        break;
                    }
                }

                /* If leaf is empty delete leaf */
                if (buckets_.size() == 0) {
                    delete this;
                    return nullptr;
                }

                return this;
            }

            size_t Depth() {
                return 0;
            }

            void RayCastProcessChild(Real_t tx0, Real_t ty0, Real_t tx1, Real_t ty1, unsigned char a, std::vector<Data>& results) {

                if (tx1 < 0 || ty1 < 0) return;

                /* If ray casting hit a leaf, add all the points to the results */
                for (size_t i = 0; i < buckets_.size(); i++)
                    results.push_back(buckets_[i].data_);

                return;
            }

        private:
            /* Holds a data point */
            struct Bucket {
                Bucket(Point2D point, Data data) : point_(point), data_(data) {};
                Point2D point_;
                Data data_;
            };
            std::deque<Bucket> buckets_;
        };

        class QuadTreeInnerNode : public QuadTreeNode {
        public:
            QuadTreeInnerNode(Point2D origin, Real_t length) {
                children_ = std::vector<QuadTreeNode *>(4, nullptr);

                this->origin_ = origin;
                this->length_ = length;
                this->type_ = QuadTreeNode::NodeType::INNER;
            }
            ~QuadTreeInnerNode() {

            }

            void Destroy() {
                for (size_t i = 0; i < children_.size(); i++) {
                    if (children_[i] != nullptr) {
                        children_[i]->Destroy();
                        delete children_[i];
                    }
                }
            }

            QuadTreeNode * Insert(Point2D point, Data data, size_t depth) {
                /* Find child */
                std::pair<Point2D, size_t> child = QuadTreeNode::FindChild(point);

                /* If null, create leaf */
                if (children_[child.second] == nullptr) {
                    children_[child.second] = new QuadTreeLeafNode(child.first, this->length_ / 2.0f);
                }

                /* Insert at that subtree */
                children_[child.second] = children_[child.second]->Insert(point, data, depth + 1);

                return this;
            }

            size_t QueryRange(AABox<2> search_box, std::vector<Data>& results) {
                
                /* Check the search box for overlapping with the quadrants of the children */
                /* Recursively call on every child, if it overlaps */

                size_t res = 0;
                for (size_t i = 0; i < children_.size(); i++) {
                    if (children_[i] != nullptr &&
                        search_box.Overlaps(children_[i]->GetAABox())) res += children_[i]->QueryRange(search_box, results);
                }

                return res;
            }

            QuadTreeNode * Remove(Point2D point) {
                /* Find child */
                std::pair<Point2D, size_t> child = QuadTreeNode::FindChild(point);

                if (children_[child.second] == nullptr) {
                    return this;
                }

                /* Remove from child */
                children_[child.second] = children_[child.second]->Remove(point);

                /* Check the number of children that you have, if zero, delete this node */
                size_t number_of_non_null_children = 0;
                for (size_t i = 0; i < children_.size(); i++)
                    if (children_[i] != nullptr) {
                        number_of_non_null_children++;
                    }

                if (number_of_non_null_children == 0) {
                    delete this;
                    return nullptr;
                }

                return this;
            }

            size_t Depth() {
                size_t current_depth = 0;
                for (size_t i = 0; i < children_.size(); i++)
                    if (children_[i] != nullptr) current_depth = std::max(current_depth, children_[i]->Depth());

                return current_depth + 1;
            }

            void RayCastProcessChild(Real_t tx0, Real_t ty0, Real_t tx1, Real_t ty1, unsigned char a, std::vector<Data>& results) {
                Real_t txm, tym;
                int current_node;

                if (tx1 < 0 || ty1 < 0) return;

                /* Calculate the middle of the entry and exit point */
                txm = Real_t(0.5)*(tx0 + tx1);
                tym = Real_t(0.5)*(ty0 + ty1);

                /* Calculate the first node to be visited */
                current_node = RayCastFirstNode(tx0, ty0, txm, tym);

                /* Iteratively visit the nodes along the ray */
                do {
                    int index = current_node ^ a;
                    switch (current_node)
                    {
                    case 0: {
                        if (children_[index] != nullptr) children_[index]->RayCastProcessChild(tx0, ty0, txm, tym, a, results);
                        current_node = RayCastNewNode(txm, 2, tym, 1);
                        break;
                    } case 1: {
                        if (children_[index] != nullptr) children_[index]->RayCastProcessChild(tx0, tym, txm, ty1, a, results);
                        current_node = RayCastNewNode(txm, 3, ty1, 4);
                        break;
                    } case 2: {
                        if (children_[index] != nullptr) children_[index]->RayCastProcessChild(txm, ty0, tx1, tym, a, results);
                        current_node = RayCastNewNode(tx1, 4, tym, 3);
                        break;
                    } case 3: {
                        if (children_[index] != nullptr) children_[index]->RayCastProcessChild(txm, tym, tx1, ty1, a, results);
                        current_node = 4;
                        break;
                    }
                    }
                } while (current_node < 4);
            }

        private:
            std::vector<QuadTreeNode *> children_;

            int RayCastFirstNode(Real_t tx0, Real_t ty0, Real_t txm, Real_t tym) {
                unsigned char answer = 0;

                if (tx0 > ty0) {
                        if (tym < tx0) answer |= 1;
                        return (int)answer;
                } else {
                        if (txm < ty0) answer |= 2;
                        return (int)answer;
                }

                return (int)answer;

            }

            int RayCastNewNode(Real_t txm, int x, Real_t tym, int y) {
                if (txm < tym) {
                    return x;
                }   
                return y;
            }

        };

    public:

        /**
            @param origin The "bottom left" point in the quad tree area
            @parma length The size of the quad tree region in all directions
        */
        QuadTree(Point2D origin, Real_t length) {
            origin_ = origin;
            length_ = length;
            root_ = new QuadTreeLeafNode(origin, length);
        }

        ~QuadTree() {
            delete root_;
        }

        /**
            Delete everything, don't use after this call
        */
        void Destroy() {
            root_->Destroy();
            delete root_;
            root_ = nullptr;
        }

        /**
            Insert a data point into the quad tree
            @param point The position in 2D space
            @param data The data to store
        */
        bool Insert(Point2D point, Data data) {
            bool inside_x = point[0] > origin_[0] && (point[0] < origin_[0] + length_);
            bool inside_y = point[1] > origin_[1] && (point[1] < origin_[1] + length_);

            if (!inside_x || !inside_y) {
                std::cout << "Point: " << point << " is outside quad tree region" << std::endl;
                return false;
            }

            root_ = root_->Insert(point, data, 0);
            return true;
        }

        /**
            Get all points inside an area
            @param search_box The searching area
            @param[out] The points inside the area
        */
        void QueryRange(AABox<2> search_box, std::vector<Data>& results) {

            if (search_box.Overlaps(AABox<2>(origin_, origin_ + Point2D(length_))))
                root_->QueryRange(search_box, results);

        }

        /**
            Remove a point from the quad tree
            @param point The 2D space point to remove
        */
        void Remove(Point2D point) {
            root_ = root_->Remove(point);

            if (root_ == nullptr) root_ = new QuadTreeLeafNode(origin_, length_);
        }

        size_t Depth() {
            return root_->Depth();
        }

        /**
            Perform ray traversal
            @param r The 2D space ray
            @param[out] results The results will be pushed back here, in first to hit order
        */
        void RayCast(Ray2D r, std::vector<Data>& results) {
            unsigned char a = 0;

            /**
                If ray has negative components calculate the reflection of the ray
            */
            if (r.Direction()[0] < 0) {
                r.Origin()[0] = (origin_[0] + length_ / 2.0) * 2 - r.Origin()[0];
                r.Direction()[0] = -r.Direction()[0];
                a |= 2;
            }
            if (r.Direction()[1] < 0) {
                r.Origin()[1] = (origin_[1] + length_ / 2.0) - r.Origin()[1];
                r.Direction()[1] = -r.Direction()[1];
                a |= 1;
            }

            /*
                Compute the starting parametric values of entry and exit for the root node
            */

            Real_t divx = Real_t(1) / r.Direction()[0];
            Real_t divy = Real_t(1) / r.Direction()[1];

            Real_t tx0 = (origin_[0] - r.Origin()[0]) * divx;
            Real_t tx1 = (origin_[0] + length_ - r.Origin()[0]) * divx;
            Real_t ty0 = (origin_[1] - r.Origin()[1]) * divy;
            Real_t ty1 = (origin_[1] + length_ - r.Origin()[1]) * divy;
            
            /* If there is intersection, continue */
            if (std::max(tx0, ty0) < std::min(tx1, ty1)) {
                root_->RayCastProcessChild(tx0, ty0, tx1, ty1, a, results);
            }
        }

    private:
        QuadTreeNode * root_;
        Point2D origin_;
        Real_t length_;
    };

}
}

#endif
