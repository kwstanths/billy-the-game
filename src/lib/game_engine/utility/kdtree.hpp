#ifndef __kdtree_hpp__
#define __kdtree_hpp__

#include <vector>
#include <exception>
#include <bitset>
#include <time.h>

/**
    A kdtree, with record of size K and type T, that holds data of type Data in each node
*/
template<int K, typename T, typename Data> 
class kdtree {
public:

    kdtree(std::vector<T> key, Data data) {
        if (key.size() != K) throw std::invalid_argument("Input must be of size K");

        key_ = key;
        data_ = data;
        discriminant_ = rand_between(0, K - 1);
        left_ = nullptr;
        right_ = nullptr;
    }

    ~kdtree() {
        if (left_ != nullptr) delete left_;
        if (right_ != nullptr) delete right_;
    }

    std::vector<T>& get_key() {
        return key_;
    }

    Data get_data() {
        return data_;
    }

    /**
        Insert element in the kdtree
        @param new_key The element to be inserted
        @param data The data to hold
        @return 0 = OK, -1 = Element has the wrong size
    */
    int insert(std::vector<T> new_key, Data data) {
        if (new_key.size() != K) return -1;

        /* If smaller, insert to left subtree */
        if (new_key[discriminant_] < key_[discriminant_]) {
            if (left_ == nullptr) left_ = new kdtree<K, T, Data>(new_key, data);
            else left_->insert(new_key, data);

            return 0;
        }

        /* If greater or equal, insert to right subtree */
        if (right_ == nullptr) right_ = new kdtree<K, T, Data>(new_key, data);
        else right_->insert(new_key, data);

        return 0;
    }

    size_t fake_insert_count_nodes(std::vector<T> new_key, Data data) {
        if (new_key.size() != K) return 0;

        /* If smaller, insert to left subtree */
        if (new_key[discriminant_] < key_[discriminant_]) {
            if (left_ == nullptr) {
                return 1;
            } else return 1 + left_->fake_insert_count_nodes(new_key, data);
        }

        /* If greater or equal, insert to right subtree */
        if (right_ == nullptr) {
            return 1;
        } else return right_->fake_insert_count_nodes(new_key, data) + 1;
    }

    /**
        Perform partical match. Example, mask = [0 1], search_key = [3 5] => query = [*, 5]
        @param mask if k-th index of mask is 1, then the k-th value of the search key must be found exactly, 0 = whatever value for the k-th element
        @param search_key The values of the partial match query. The indices that the mask is 0 are ignored, whatever the value
        @param[out] output The output holds pointers to the records in the kdtree. Please don't use these pointers for insertions or other searches
        @return 0 = OK, -1 = search_key has the wrong size
    */
    int partial_match(std::bitset<K> mask, std::vector<T> search_key, std::vector<kdtree<K,T,Data> * > & output) {
        if (search_key.size() != K) return -1;

        /* Check if this node's key should be included */
        if (check_match(mask, search_key)) output.push_back(this);

        /* Go to children */
        bool value_set = mask[discriminant_];
        if (value_set) {
            /* If the value is set to be exactly matched, go to either left or right subtree */
            if (search_key[discriminant_] < key_[discriminant_]) {
                if (left_ != nullptr) left_->partial_match(mask, search_key, output);
                return 0;
            }

            if (right_ != nullptr) right_->partial_match(mask, search_key, output);
            return 0;
        }

        /* If the value is set to whatever match */
        if (left_ != nullptr) left_->partial_match(mask, search_key, output);
        if (right_ != nullptr) right_->partial_match(mask, search_key, output);

        return 0;
    }

    int fake_partial_match_count_nodes(std::bitset<K> mask, std::vector<T> search_key) {
        if (search_key.size() != K) return -1;

        size_t nodes_visited = 1;
        /* Go to children */
        bool value_set = mask[discriminant_];
        if (value_set) {
            /* If the value is set to be exactly matched, go to either left or right subtree */
            if (search_key[discriminant_] < key_[discriminant_]) {
                if (left_ != nullptr) nodes_visited += left_->fake_partial_match_count_nodes(mask, search_key);
                return nodes_visited;
            }

            if (right_ != nullptr) nodes_visited += right_->fake_partial_match_count_nodes(mask, search_key);
            return nodes_visited;
        }

        /* If the value is set to whatever match */
        if (left_ != nullptr)  nodes_visited += left_->fake_partial_match_count_nodes(mask, search_key);
        if (right_ != nullptr) nodes_visited += right_->fake_partial_match_count_nodes(mask, search_key);

        return nodes_visited;
    }

    /**
        Perform orthogonal match, Example search all keys in the region [a1, b1] x [a2, b2] x ... x [ak, bk]
        @param query A vector of pairs for the ranges, query must be of size K, please let ai <= bi
        @param[out] output The output holds pointers to the records of the tree. Please don't use these pointers for insertions or other searches
        @return 0 = OK, -1 = query has the wrong size
    */
    int orthogonal_match(std::vector<std::pair<T, T>> query, std::vector<kdtree<K, T, Data> * > & output) {
        if (query.size() != K) return -1;

        /* Check if current node matches */
        if (check_match(query)) output.push_back(this);

        /* Check if we have to only to the left */
        if (key_[discriminant_] > query[discriminant_].second) {
            if (left_ != nullptr) left_->orthogonal_match(query, output);
            return 0;
        }
        
        /* Check if we have to only to the right */
        if (key_[discriminant_] < query[discriminant_].first) {
            if (right_ != nullptr) right_->orthogonal_match(query, output);
            return 0;
        }

        /* else we have to go to both */
        if (left_ != nullptr) left_->orthogonal_match(query, output);
        if (right_ != nullptr) right_->orthogonal_match(query, output);

        return 0;
    }

private:
    std::vector<T> key_;
    Data data_;
    size_t discriminant_;
    kdtree<K, T, Data> * left_, * right_;

    int rand_between(int min, int max) {
        return rand() % (max - min + 1) + min;
    }

    bool check_match(std::bitset<K>& mask, std::vector<T>& search_key) {
        for (size_t i = 0; i < K; i++)
            if (mask[i] && search_key[i] != key_[i]) return false;

        return true;
    }

    bool check_match(std::vector<std::pair<T, T>> query) {
        for (size_t i = 0; i < K; i++)
            if (key_[i] < query[i].first || key_[i] > query[i].second) return false;

        return true;
    }
    
};

#endif