#ifndef __HASHTABLE_HPP__
#define __HASHTABLE_HPP__

#include <iostream>
#include <vector>

#include "List.hpp"


namespace game_engine {

namespace utility {

      template<typename Key, typename Value> class HashTable{
      private:

            struct HashElement_t {
                  Key key_;
                  Value value_;
                  
                  HashElement_t(Key key, Value value): key_(key), value_(value) {};

                  bool operator==(const HashElement_t& other) {
                        key_ == other.key_;
                  }
            };

            std::vector<List<HashElement_t> * > hashtable_;
            unsigned long int size_, elements_;
            double max_load_;

            /**
                  Double the size of the hashtable and re-insert all elements
            */
            void Rehash(){
            //       size = 2 * size;
            //       std::vector<list::list<T> * > * temp;
            //       temp = hashtable;
            //       hashtable = new std::vector<list::list<T> * >(size, NULL);
            //       elements = 0;
            //       for(typename std::vector<list::list<T> *>::iterator itr = temp->begin(); itr!=temp->end(); ++itr){
            //             if (*itr != NULL)
            //                   for(typename list::list<T>::iterator itr2 = (*itr)->begin(); itr2 != (*itr)->end(); itr2+=1) {
            //                         Insert(*itr2);
            //                   }
            //       }
            //       delete temp;
            }

            /**
                  Get the hash value for an element
                  @param key the value to get the hash
                  @return The hash value
            */
            unsigned long int GetHash(Key key) const {
                  return std::hash<Key>{}(key) % size_;
            }

      public:
            /*
                  Initialize a hash table with size _size
                  No matter how many elements you insert, the hashtable's size won't change
            */
            HashTable(unsigned long int size){
                  if (size <= 0) throw std::out_of_range("Hash table size must be a positive number");
                  hashtable_ = std::vector<List<HashElement_t> * >(size, NULL);
                  size_ = size;
                  elements_ = 0;
                  max_load_ = -1;
            }

            /*
                  Initialize a hash table with size _size and max load (elements/size) equal to _max_load
                  When an element is inserted if elements/size > _max_load then a rehashing will occur with
                  the new hashtable size equal to double the previous size
            */
            HashTable(unsigned long int size, double max_load){
                  if (size <= 0) throw std::out_of_range("Hash table size must be a positive number");
                  hashtable_ = std::vector<List<HashElement_t> * >(size, NULL);
                  size_ = size;
                  elements_ = 0;
                  max_load_ = max_load;
            }

            ~HashTable(){
                  //Clear();
            }

            /*
                  Insert an element to the hashtable. If rehashing is enabled when HashTable was constructed then this operation
                  might take more time then expected in some cases.
                  @param _value The value to be inserted
                  @return true = Ok
            */
            bool Insert(Key key, Value value) {
                  unsigned long int hash_value = GetHash(key);

                  HashElement_t new_elem(key, value);
                  bool ret = false;
                  if (hashtable_.at(hash_value) == NULL) {
                        hashtable_.at(hash_value) = new List<HashElement_t>(new_elem);
                        ret = true;
                  } else {
                        List<HashElement_t> * hashed_elems = hashtable_.at(hash_value);
                        if (hashed_elems->FindForward(new_elem) == hashed_elems->end()){
                              /* If element does not exist in the bucket list */
                              hashed_elems->PushTop(new_elem);
                              ret = true;
                        }

                        /* If it exists, dont do anything */
                  }
                  
                  if (ret) elements_++;

                  if ((max_load_ > 0) && (GetLoad() >= max_load_)) Rehash();
            }

            // /*
            //       Search for an element in the HashTable
            //       @param _value The value to be searched
            //       @return true = Found, false = Not found
            // */
            // bool Find(T _value) const {
            //       unsigned long int hash_value = GetHash(_value);

            //       if (hashtable->at(hash_value) == NULL) return false;
            //       int ret = hashtable->at(hash_value)->FindForward(_value);

            //       if (ret == -1) return false;
            //       return true;
            // }

            // /*
            //       Remove an element from the hash table
            //       @param _value The value to be removed
            //       @return true = Removed, false = Not found
            // */
            // bool Remove(T _value) {
            //       unsigned long int hash_value = GetHash(_value);

            //       bool ret = false;
            //       if (hashtable->at(hash_value) != NULL) ret = hashtable->at(hash_value)->Remove(_value);
            //       if (ret) elements--;

            //       return false;
            // }

            /*
                  Get hash table's current size. If rehashing is enabled then this value might differ from the one given
                  during object construction
                  @return The size
            */
            unsigned long int GetSize() const {
                  return size_;
            }

            /*
                  Get number of elements inserted in the hash table
                  @return Number of elements
            */
            unsigned long int GetNumberOfElements() const {
                  return elements_;
            }

            /*
                  Get the load of the hash table as (inserted elements/ size)
                  @return Table load
            */
            double GetLoad() const {
                  return (1.0*elements_) / (1.0*size_);
            }

            /*
                  Prints the hash table in a relatively nice format
            */
            void PrettyPrint() const {
                  std::cout << "Hash table:" << std::endl;
                  for(typename std::vector<List<HashElement_t> * >::const_iterator itr = hashtable_.begin(); itr!=hashtable_.end(); ++itr){
                        if (*itr != NULL)
                              for (typename List<HashElement_t>::iterator itr2 = (*itr)->begin(); itr2 != (*itr)->end(); ++itr2) 
                              std:: cout << (*itr2).value_ << " -> ";
                        std::cout << "null" << std::endl;
                  }
            }

            // /*
            //       Removes all the inserted elements
            // */
            // void Clear(){
            //       for(typename std::vector<list::list<T> *>::iterator itr = hashtable->begin(); itr!=hashtable->end(); ++itr) {
            //             delete *itr;
            //             *itr = NULL;
            //       }
            //       elements = 0;
            // }
      };

}
}

#endif
