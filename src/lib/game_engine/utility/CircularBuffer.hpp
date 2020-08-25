#ifndef __CircularBuffer_hpp__
#define __CircularBuffer_hpp__

#include <vector>

#include "game_engine/math/HelpFunctions.hpp"

namespace game_engine {
namespace utility {

    /**
        A circular FIFO buffer to store items sequentially
    */
    template<typename T> class CircularBuffer {
    public:
        /**
            Does nothing in particular, call Init()
        */
        CircularBuffer() {
            is_inited_ = false;
        }
    
        /**
            Initializes the objects
            @param buffer_size The maximum number of items to store. Will be rounded up to the next power of two number, e.g 58 -> 64
            @return 0 = OK
        */
        int Init(size_t buffer_size) {
            /**
                Find the next power of two for buffer size, so that we can avoid the modulo operatiion
            */
            size_t size_power_of_two = game_engine::math::PowerOfTwo(buffer_size);
            /**
                Find the AND mask for the "modulo" operation
            */
            modulo_mask = size_power_of_two - 1;
    
            buffer_ = std::vector<T>(size_power_of_two);
            buffer_index_start_ = 0;
            buffer_index_stop_ = 0;
            items_inside_ = 0;
    
            is_inited_ = true;
            return 0;
        }
        
        /**
            Destroy the buffer. Call Init() again
            @return 0 = OK
        */
        int Destroy() {
            buffer_.clear();
            items_inside_ = 0;
            buffer_index_start_ = 0;
            buffer_index_stop_ = 0;

            is_inited_ = false;
            return 0;
        }
        
        /**
            Get whether or not the object is initialised
            @return true = initialised, false = not initialised
        */
        bool IsInited() {
            return is_inited_;
        }
        
        /**
            Get the maximum number of items possible
            @return The maximum number of items possible
        */
        size_t Size() {
            return buffer_.size();
        }
    
        /**
            Get the number of items actially stored
            @return The number of items actially stored
        */
        size_t Items() {
            return items_inside_;
        }
    
        /**
            Get whether or not the buffer is full
            @return true = full, false = not full
        */
        bool IsFull() {
            return items_inside_ >= buffer_.size();
        }

        /**
            Removes all items, keeps the same size
        */
        void Clear() {
            items_inside_ = 0;
            buffer_index_start_ = 0;
            buffer_index_stop_ = 0;
        }
    
        /**
            Push an item to the buffer
            @param item The item
            @return 0 = OK, -1 = Not initialised or full
        */
        int Push(T item) {
            if (!is_inited_) return -1;
            if (items_inside_ >= buffer_.size()) return -1;
    
            buffer_[buffer_index_stop_] = item;
            buffer_index_stop_ = (buffer_index_stop_ + 1) & modulo_mask;
            items_inside_++;
    
            return 0;
        }
    
        /**
            Get an item from the buffer
            @param[out] out_item Output for the extracted item
            @return 0 = OK, -1 = not initialised or no items
        */
        int Get(T& out_item) {
            if (!is_inited_) return -1;
            if (items_inside_ == 0) return -1;
    
            out_item = buffer_[buffer_index_start_];
            buffer_index_start_ = (buffer_index_start_ + 1) & modulo_mask;
            items_inside_--;
    
            return 0;
        }

    private:
        bool is_inited_;
    
        std::vector<T> buffer_;
        size_t modulo_mask;
    
        size_t items_inside_;
        size_t buffer_index_start_;
        size_t buffer_index_stop_;

        class CBiterator : public std::iterator<std::forward_iterator_tag, T> {
            friend class CircularBuffer;
        private:
            std::vector<T>& buffer_;
            int index_ = -1;

            /* Start and stop indices included as valid vector indices */
            int start_ = -1;
            int stop_ = -1;
            CBiterator(int start, int stop, std::vector<T>& buffer) : start_(start), stop_(stop), buffer_(buffer) {
                index_ = start;
            };

        public:
            T& operator*() {
                return buffer_[index_];
            }

            const CBiterator& operator++() {

                index_++;
                index_ = index_ % buffer_.size();
                if (index_ == stop_) index_ = -1;

                return *this;
            }

            bool operator!=(const CBiterator& other) const {
                return this->index_ != other.index_;
            }

            bool operator==(const CBiterator& other) const {
                return this->index_ == other.index_;
            }

        };

    public:
        typedef CBiterator iterator;

        CBiterator begin() {
            if (buffer_index_start_ == buffer_index_stop_) return end();
            return CBiterator(buffer_index_start_, buffer_index_stop_, buffer_);
        }
        
        CBiterator end() {
            return CBiterator(-1, buffer_index_stop_, buffer_);
        }
    };

}
}

#endif