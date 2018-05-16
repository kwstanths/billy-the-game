#ifndef __HashTable_HPP__
#define __HashTable_HPP__

#include <iostream>
#include <vector>

#include "List.hpp"


namespace game_engine {

namespace utility {

      /**
            TODO Implement iterator operators
      */
      template<typename Key, typename Value> class HashTable{
      private:

            class HashElement {
                  friend class HashTable;
            private:
                  Key key_;
                  Value value_;
            public:
                  HashElement(Key key, Value value): key_(key), value_(value) {};

                  bool operator==(const HashElement& other) {
                        return key_ == other.key_;
                  }
            };

            class HashIterator : public std::iterator<std::forward_iterator_tag, Key, Value> {
			friend class HashTable;
			friend class HashElement;
		private:
			HashElement * pointed_;
                  const std::vector<List<HashElement> * > * hashtable_;
                  size_t pointed_index_;
			HashIterator(HashElement * pointed, 
                        const std::vector<List<HashElement> * > * hashtable, 
                        size_t index): pointed_(pointed), pointed_index_(index), hashtable_(hashtable) { };

		public:
                  Key& GetKey() {
                        return pointed_->key_;
                  }

			Value& GetValue() {
				return pointed_->value_;
			}

			const HashIterator& operator++(){

                        /* Find the next in the current bucket list */
                        List<HashElement> * current_bucket_list = hashtable_->at(pointed_index_);
                        typename List<HashElement>::iterator next = current_bucket_list->FindForward(*pointed_);
                        ++next;
                        if (next == current_bucket_list->end()){
                              /* Find the next bucket list */
                              pointed_index_ ++;
                              while(pointed_index_ < hashtable_->size() && hashtable_->at(pointed_index_) == nullptr) 
                                    pointed_index_++;

                              /* If none exists return the iterator end */
                              if (pointed_index_ == hashtable_->size()) {
                                    pointed_ = nullptr;
                                    return *this;
                              }

                              /* if not, return the first from the new list */
                              pointed_ = hashtable_->at(pointed_index_)->PeekTop();
				      return *this;
                        }
                        
                        pointed_ = &(*next);
                        return *this;

                        
			}

			// const HashIterator& operator+=(int amount){
			// 	int i = 0;
			// 	while(i++ < amount) {
			// 		pointed_ = pointed_->next;
			// 	}
			// }

			bool operator!=(const HashIterator& other) const {
				return this->pointed_ != other.pointed_;
			}

			bool operator==(const HashIterator& other) const {
				return this->pointed_ == other.pointed_;
			}
		};

            std::vector<List<HashElement> * > hashtable_;
            size_t size_, elements_;
            double max_load_;

            /**
                  Double the size of the hashtable and re-insert all the elements
            */
            void Rehash() {
                  size_ = 2 * size_;
                  std::vector<List<HashElement> *> temp;
                  temp = hashtable_;
                  hashtable_ = std::vector<List<HashElement> *> (size_, nullptr);
                  elements_ = 0;
                  for(typename std::vector<List<HashElement> *>::iterator itr = temp.begin(); itr!=temp.end(); ++itr){
                        if (*itr != nullptr)
                              for(typename List<HashElement>::iterator itr2 = (*itr)->begin(); itr2 != (*itr)->end(); ++itr2) {
                                    Insert((*itr2).key_, (*itr2).value_);
                              }
                  }
            }

            /**
                  Get the hash value for a key
                  @param key The key to get the hash
                  @return The hash value
            */
            size_t GetHash(Key key) const {
                  return std::hash<Key>{}(key) % size_;
            }

      public:
            /**
                  The HashTable's iterator
            */
            typedef HashIterator iterator;

            /*
                  Initialize a hash table with size
                  No matter how many elements you insert, the hashtable's size won't change
            */
            HashTable(size_t size){
                  if (size <= 0) throw std::out_of_range("Hash table size must be a positive number");
                  hashtable_ = std::vector<List<HashElement> * >(size, nullptr);
                  size_ = size;
                  elements_ = 0;
                  max_load_ = -1;
            }

            /*
                  Initialize a hash table with size and max load (elements/size) equal to max_load
                  When an element is inserted if elements/size > max_load then a rehashing will occur 
                  with the new hashtable size equal to double the previous size
            */
            HashTable(size_t size, double max_load){
                  if (size <= 0) throw std::out_of_range("Hash table size must be a positive number");
                  hashtable_ = std::vector<List<HashElement> * >(size, nullptr);
                  size_ = size;
                  elements_ = 0;
                  max_load_ = max_load;
            }

            ~HashTable(){
                  Clear();
            }

            /*
                  Insert an element to the hashtable. If rehashing is enabled when HashTable was constructed 
                  then this operation might take more time then expected in some cases.
                  @param key The key of the value to be inserted
                  @param value The value to be inserted
                  @return true = Ok, false = Already inserted
            */
            bool Insert(Key key, Value value) {
                  size_t hash_value = GetHash(key);
                  
                  HashElement new_elem(key, value);
                  bool ret = false;
                  if (hashtable_.at(hash_value) == nullptr) {
                        hashtable_.at(hash_value) = new List<HashElement>(new_elem);
                        ret = true;
                  } else {
                        List<HashElement> * hashed_elems = hashtable_.at(hash_value);
                        
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

            /**
                  Get the beginning of the iterator
            */
            HashIterator begin() {
                  size_t i = 0;
                  while(i < hashtable_.size() && hashtable_.at(i) == nullptr) i++;
                  if (i == hashtable_.size()) return HashIterator(nullptr, &hashtable_, hashtable_.size());

                  HashElement * first = hashtable_.at(i)->PeekTop();
                  return HashIterator(first, &hashtable_, i);
            }

            /**
                  Get the end of the iterator. Incrementing or derefercing, or in any way using it,
                  expect for comparison, is not recommended            
            */
            HashIterator end() {
                  return HashIterator(nullptr, &hashtable_, hashtable_.size());
            }

            /*
                  Search for an element in the HashTable, if not found you get the end() of the 
                  iterator
                  @param key The key to be searched
                  @return An iterator to the element, use GetKey(), and GetValue()
            */
            HashIterator Find(Key key) const {
                  size_t hash_value = GetHash(key);

                  if (hashtable_.at(hash_value) == nullptr) return HashIterator(nullptr, &hashtable_, hashtable_.size());

                  List<HashElement> * hashed_elems = hashtable_.at(hash_value);
                  typename List<HashElement>::iterator itr = hashed_elems->FindForward(HashElement(key, Value()));
                  if (itr != hashed_elems->end()) {
                        return HashIterator(&(*itr), &hashtable_, hash_value);
                  }

                  return HashIterator(nullptr, &hashtable_, hashtable_.size());
            }

            /*
                  Remove an element from the hash table
                  @param key The key of the element to remove
                  @return true = Removed, false = Not found
            */
            bool Remove(Key key) {
                  size_t hash_value = GetHash(key);

                  bool ret = false;
                  if (hashtable_.at(hash_value) != NULL) 
                        ret = hashtable_.at(hash_value)->Remove(HashElement(key, Value()));
                  
                  if (ret) {
                        elements_--;
                        return false;
                  }

                  return true;
            }

            /*
                  Get hash table's current size. If rehashing is enabled then this value might differ from the one given
                  during object construction
                  @return The size
            */
            size_t GetSize() const {
                  return size_;
            }

            /*
                  Get number of elements inserted in the hash table
                  @return Number of elements
            */
            size_t GetNumberOfElements() const {
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
                  for(typename std::vector<List<HashElement> * >::const_iterator itr = hashtable_.begin(); itr!=hashtable_.end(); ++itr){
                        if (*itr != nullptr)
                              for (typename List<HashElement>::iterator itr2 = (*itr)->begin(); itr2 != (*itr)->end(); ++itr2) 
                                    std:: cout << "(" << (*itr2).key_ << "," << (*itr2).value_ << ")" << " -> ";
                        std::cout << "null" << std::endl;
                  }
            }

            /*
                  Removes all the inserted elements in the table
            */
            void Clear(){
                  for(typename std::vector<List<HashElement> *>::iterator itr = hashtable_.begin(); itr!=hashtable_.end(); ++itr) {
                        delete *itr;
                        *itr = nullptr;
                  }
                  elements_ = 0;
            }
      };

}
}

#endif
