#ifndef __kquaddtree_hpp__
#define __kquaddtree_hpp__

#include <vector>
#include <exception>
#include <bitset>
#include <time.h>

/**
    A kd quad tree, with record of size K and type T, that holds data of type Data in each node
*/
template<int K, typename T, typename Data>
class kdquadtree {
public:

    kdquadtree(std::vector<T> key, Data data) {
        if (key.size() != K) throw std::invalid_argument("Input must be of size K");

        key_ = key;
        data_ = data;
        
        /* Left shift 1 K times, to calcluate the pow(2,K) */
        number_of_children_ = 1 << K;

        children_ = std::vector< kdquadtree<K, T, Data> * >(number_of_children_, nullptr);
    }

    ~kdquadtree() {
        for (size_t i = 0; i < number_of_children_; i++)
            if (children_[i] != nullptr) delete children_[i];
    }

    std::vector<T>& get_key() {
        return key_;
    }

    Data get_data() {
        return data_;
    }

    /**
        Insert element in the tree
        @param new_key The element to be inserted
        @param data The data to hold
        @return 0 = OK, -1 = Element has the wrong size
    */
    int insert(std::vector<T> new_key, Data data) {
        if (new_key.size() != K) return -1;

        std::bitset<K> bitstring(0);
        for (size_t i = 0; i < K; i++) {
            if (new_key[i] >= key_[i]) bitstring[i] = 1;
        }

        size_t child = bitstring.to_ulong();

        if (children_[child] == nullptr) children_[child] = new kdquadtree<K, T, Data>(new_key, data);
        else children_[child]->insert(new_key, data);

        return 0;
    }

    size_t fake_insert_count_nodes(std::vector<T> new_key, Data data) {
        if (new_key.size() != K) return -1;

        std::bitset<K> bitstring(0);
        for (size_t i = 0; i < K; i++) {
            if (new_key[i] >= key_[i]) bitstring[i] = 1;
        }

        size_t child = bitstring.to_ulong();

        if (children_[child] == nullptr) {
            return 1;
        } else return 1 + children_[child]->fake_insert_count_nodes(new_key, data);

        return 0;
    }

    /**
        Perform partical match. Example, mask = [0 1], search_key = [3 5] => query = [*, 5]
        @param mask if k-th index of mask is 1, then the k-th value of the search key must be found exactly, 0 = whatever value for the k-th element
        @param search_key The values of the partial match query. The indices that the mask is 0 are ignored, whatever the value
        @param[out] output The output holds pointers to the records in the kdtree. Please don't use these pointers for insertions or other searches
        @return 0 = OK, -1 = search_key has the wrong size
    */
    int partial_match(std::bitset<K> mask, std::vector<T> search_key, std::vector<kdquadtree<K, T, Data> * > & output) {
        if (search_key.size() != K) return -1;

        /* Check if this node's key should be included */
        if (check_match(mask, search_key)) output.push_back(this);

        /* Iterate over all possible children */
        for (size_t i = 0; i < number_of_children_; i++) {
            std::bitset<K> temp(i);
            for (size_t key_bit = 0; key_bit < K; key_bit++) {
                if (mask[key_bit]) temp[key_bit] = (search_key[key_bit] >= key_[key_bit]);
            }
            if (temp.to_ulong() == i)
                if (children_[i] != nullptr) children_[i]->partial_match(mask, search_key, output);
        }

        return 0;
    }

    size_t fake_partial_match_count_nodes(std::bitset<K> mask, std::vector<T> search_key) {
        if (search_key.size() != K) return -1;

        size_t nodes_visited = 1;
        /* Iterate over all possible children */
        for (size_t i = 0; i < number_of_children_; i++) {
            std::bitset<K> temp(i);
            for (size_t key_bit = 0; key_bit < K; key_bit++) {
                if (mask[key_bit]) temp[K - 1 - key_bit] = (search_key[key_bit] >= key_[key_bit]);
            }
            if (temp.to_ulong() == i)
                if (children_[i] != nullptr) nodes_visited += children_[i]->fake_partial_match_count_nodes(mask, search_key);
        }

        return nodes_visited;
    }
    
    /**
        Perform orthogonal match, Example search all keys in the region [a1, b1] x [a2, b2] x ... x [ak, bk]
        @param query A vector of pairs for the ranges, query must be of size K, please let ai <= bi
        @param[out] output The output holds pointers to the records of the tree. Please don't use these pointers for insertions or other searches
        @return 0 = OK, -1 = query has the wrong size
    */
    int orthogonal_match(std::vector<std::pair<T, T>> query, std::vector<kdquadtree<K, T, Data> * > & output) {
        if (query.size() != K) return -1;

        if (check_match(query)) output.push_back(this);

        for (size_t i = 0; i < number_of_children_; i++) {
            
            std::bitset<K> temp(i);
            bool visit_child = true;
            for (size_t key_bit = 0; key_bit < K; key_bit++)
                if (temp[key_bit] && query[key_bit].second < key_[key_bit])
                    visit_child = false;
                else if (!temp[key_bit] && query[key_bit].first >= key_[key_bit])
                    visit_child = false;

            if (visit_child && children_[i] != nullptr) children_[i]->orthogonal_match(query, output);
        }

        return 0;
    }

private:
    size_t number_of_children_;
    std::vector<T> key_;
    Data data_;
    std::vector< kdquadtree<K, T, Data> * > children_;

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