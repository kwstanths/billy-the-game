#ifndef __QuadTreeBoxes_hpp__
#define __QuadTreeBoxes_hpp__

#include <vector>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <bitset>

#include "game_engine/math/Types.hpp"
#include "game_engine/math/AABox.hpp"

#include "debug_tools/Assert.hpp"
#include "debug_tools/Console.hpp"

namespace mth = game_engine::math;
namespace dt = debug_tools;

using namespace game_engine::math;

namespace game_engine { namespace utility {

    /**
        A Quad tree data structure that holds 2D boxes, to accelereate ray casting in 2D space
        BUCKET_SIZE = How many boxes to hold at last depth
        MAX_DEPTH = The max depth of the tree
    */
    template<typename Data, int BUCKET_SIZE = 1, int MAX_DEPTH = 13>
    class QuadTreeBoxes {
    private:
        class QuadTreeBoxesNode {
        public:
            enum NodeType {
                LEAF,
                INNER,
            };
    
            virtual ~QuadTreeBoxesNode() {
    
            }
    
            NodeType GetNodeType() { return type_; }
    
            AABox<2> GetAABox() {
                return AABox<2>(origin_, origin_ + Vector2D(length_));
            }
    
            virtual void Destroy() = 0;
            virtual QuadTreeBoxesNode * Insert(Data data, AABox<2> box, std::unordered_map<Data, AABox<2>>& boxes, size_t depth) = 0;
            virtual size_t Depth() = 0;
    
            virtual bool RayCastProcessChild(Real_t tx0, Real_t ty0, Real_t tx1, Real_t ty1, unsigned char a, Ray2D& ray, std::unordered_map<Data, AABox<2>>& boxes, std::vector<Data>& results) = 0;
    
        protected:
            NodeType type_;
            Vector2D origin_;
            Real_t length_;
    
            /**
               Find the index of the child, and the new orign
            */
            std::pair<Vector2D, size_t> FindChild(Vector2D& point) {
                /**
                    Find in which quadrant this point lies to, and what's the origin
                    point of this quadrant. Return both
                */
                size_t index = 0;
                Real_t H = length_ / 2.0f;
                Vector2D new_node_origin = origin_;
    
                Vector2D center = origin_ + Vector2D(H);
                Vector2D V = point - center;
    
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
    
            Vector2D GetQuadrantOrigin(short quadrant) {
                Vector2D new_node_origin = origin_;
    
                std::bitset<2> bits(quadrant);
                Real_t H = length_ / 2.0f;
                new_node_origin[1] += H * bits[0];
                new_node_origin[0] += H * bits[1];
    
                return new_node_origin;
            }
    
        };
    
        class QuadTreeBoxesLeafNode : public QuadTreeBoxesNode {
        public:
            QuadTreeBoxesLeafNode(Vector2D origin, Real_t length) {
                this->origin_ = origin;
                this->length_ = length;
                this->type_ = QuadTreeBoxesNode::NodeType::LEAF;
            }
    
            void Destroy() {
    
            }
    
            QuadTreeBoxesNode * Insert(Data data, AABox<2> box, std::unordered_map<Data, AABox<2>>& boxes, size_t depth) {
                if (!box.Overlaps(AABox<2>(origin_, origin_ + length_))) return this;
    
                if (buckets_.size() < BUCKET_SIZE || depth >= MAX_DEPTH) {
                    buckets_.push_back(data);
                    return this;
                }
    
                QuadTreeBoxesInnerNode * temp = new  QuadTreeBoxesInnerNode(this->origin_, this->length_);
                for (size_t i = 0; i < buckets_.size(); i++) {
                    std::unordered_map<Data, AABox<2>>::iterator itr = boxes.find(buckets_[i].data_);
                    temp->Insert(buckets_[i].data_, itr->second, boxes, depth + 1);
                }
                temp->Insert(data, box, boxes, depth + 1);
    
                delete this;
                return temp;
            }
    
            size_t Depth() {
                return 0;
            }
    
            bool RayCastProcessChild(Real_t tx0, Real_t ty0, Real_t tx1, Real_t ty1, unsigned char a, Ray2D& ray, std::unordered_map<Data, AABox<2>>& boxes, std::vector<Data>& results) {
    
                if (tx1 < 0 || ty1 < 0) return false;
    
                for (size_t i = 0; i < buckets_.size(); i++) {
                    AABox<2> box = boxes[buckets_[i].data_];
                    Real_t t;
                    bool ret = IntersectionAABoxRay2D(box, ray, t);
                    /* 
                        t < 1: Make sure that the intersection of the ray with the box is within distance 1, Don't return intersection that
                        are further away
                    */
                    if (ret && t < 1) {
                        results.push_back(buckets_[i].data_);
                        return true;
                    }
                }
    
                return false;
            }
    
        private:
            /* Holds a data point */
            struct Bucket {
                Bucket(Data data) : data_(data) {};
                Data data_;
            };
            std::deque<Bucket> buckets_;
        };
    
        class QuadTreeBoxesInnerNode : public QuadTreeBoxesNode {
        public:
            QuadTreeBoxesInnerNode(Vector2D origin, Real_t length) {
                children_ = std::vector<QuadTreeBoxesNode *>(4, nullptr);
    
                this->origin_ = origin;
                this->length_ = length;
                this->type_ = QuadTreeBoxesNode::NodeType::INNER;
            }
            ~QuadTreeBoxesInnerNode() {
    
            }
    
            void Destroy() {
                for (size_t i = 0; i < children_.size(); i++) {
                    if (children_[i] != nullptr) {
                        children_[i]->Destroy();
                        delete children_[i];
                    }
                }
            }
    
            QuadTreeBoxesNode * Insert(Data data, AABox<2> box, std::unordered_map<Data, AABox<2>>& boxes, size_t depth) {
    
                Real_t H = this->length_ / 2.0f;
                for (size_t i = 0; i < children_.size(); i++) {
                    Vector2D child_origin = QuadTreeBoxesNode::GetQuadrantOrigin(static_cast<short>(i));
                    if (box.Overlaps(AABox<2>(child_origin, child_origin + H))) {
                        if (children_[i] == nullptr) children_[i] = new  QuadTreeBoxesLeafNode(child_origin, H);
                        children_[i] = children_[i]->Insert(data, box, boxes, depth + 1);
                    }
                }
    
                return this;
            }
    
            size_t Depth() {
                size_t current_depth = 0;
                for (size_t i = 0; i < children_.size(); i++)
                    if (children_[i] != nullptr) current_depth = std::max(current_depth, children_[i]->Depth());
    
                return current_depth + 1;
            }
    
            bool RayCastProcessChild(Real_t tx0, Real_t ty0, Real_t tx1, Real_t ty1, unsigned char a, Ray2D& ray, std::unordered_map<Data, AABox<2>>& boxes, std::vector<Data>& results) {
                Real_t txm, tym;
                int current_node;
    
                if (tx1 < 0 || ty1 < 0) return false;
    
                /* TODO: Thoroughly debug this */
                if (!QuadTreeBoxesNode::GetAABox().Inside(ray.Origin()) && std::abs(tx0) > 5 && std::abs(ty0) > 5) return false;
    
                /* Calculate the middle of the entry and exit point */
                txm = Real_t(0.5)*(tx0 + tx1);
                tym = Real_t(0.5)*(ty0 + ty1);
    
                /* Calculate the first node to be visited */
                current_node = RayCastFirstNode(tx0, ty0, txm, tym);
    
                bool hit = false;
                /* Iteratively visit the nodes along the ray */
                do {
                    int index = current_node ^ a;
                    switch (current_node)
                    {
                    case 0: {
                        if (children_[index] != nullptr) hit = hit | children_[index]->RayCastProcessChild(tx0, ty0, txm, tym, a, ray, boxes, results);
                        current_node = RayCastNewNode(txm, 2, tym, 1);
                        break;
                    } case 1: {
                        if (children_[index] != nullptr) hit = hit | children_[index]->RayCastProcessChild(tx0, tym, txm, ty1, a, ray, boxes, results);
                        current_node = RayCastNewNode(txm, 3, ty1, 4);
                        break;
                    } case 2: {
                        if (children_[index] != nullptr) hit = hit | children_[index]->RayCastProcessChild(txm, ty0, tx1, tym, a, ray, boxes, results);
                        current_node = RayCastNewNode(tx1, 4, tym, 3);
                        break;
                    } case 3: {
                        if (children_[index] != nullptr) hit = hit | children_[index]->RayCastProcessChild(txm, tym, tx1, ty1, a, ray, boxes, results);
                        current_node = 4;
                        break;
                    }
                    }
                    if (hit) return true;
                } while (current_node < 4);
    
                return hit;
            }
    
        private:
            std::vector<QuadTreeBoxesNode *> children_;
    
            int RayCastFirstNode(Real_t tx0, Real_t ty0, Real_t txm, Real_t tym) {
                unsigned char answer = 0;
    
                if (tx0 > ty0) {
                    if (tym < tx0) answer |= 1;
                    return (int)answer;
                }
                else {
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
        QuadTreeBoxes(Vector2D origin, Real_t length) {
            origin_ = origin;
            length_ = length;
            root_ = new QuadTreeBoxesLeafNode(origin, length);
        }
    
        ~QuadTreeBoxes() {
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
        bool Insert(Data data, AABox<2> box) {
    
            boxes_[data] = box;
            if (!box.Overlaps(root_->GetAABox())) false;
    
            root_ = root_->Insert(data, box, boxes_, 0);
            return true;
        }
    
        size_t Depth() {
            return root_->Depth();
        }
    
        /**
            Perform ray traversal
            @param r The 2D space ray
            @param[out] results The results will be pushed back here, in first to hit order
        */
        bool RayCast(Ray2D r, std::vector<Data>& results) {
            Ray2D original_ray = r;
    
            unsigned char a = 0;
    
            /**
                If ray has negative components calculate the reflection of the ray
            */
            if (r.Direction()[0] < 0) {
                r.Origin()[0] = (origin_[0] + length_ / 2.0f) * 2.0f - r.Origin()[0];
                r.Direction()[0] = -r.Direction()[0];
                a |= 2;
            }
            if (r.Direction()[1] < 0) {
                r.Origin()[1] = (origin_[1] + length_ / 2.0f) - r.Origin()[1];
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
                return root_->RayCastProcessChild(tx0, ty0, tx1, ty1, a, original_ray, boxes_, results);
            }
    
            return false;
        }
    
    private:
        QuadTreeBoxesNode * root_;
        Vector2D origin_;
        Real_t length_;
    
        std::unordered_map<Data, AABox<2>> boxes_;
    };

}
}

#endif