#ifndef __CircularBuffer_hpp__
#define __CircularBuffer_hpp__

#include <vector>

#include "game_engine/math/HelpFunctions.hpp"

namespace game_engine {
namespace utility {


    template<typename T> class CircularBuffer {
    public:
        CircularBuffer() {
            is_inited_ = false;
        }
    
        int Init(size_t buffer_size) {
            size_t size_power_of_two = game_engine::math::PowerOfTwo(buffer_size);
            modulo_mask = size_power_of_two - 1;
    
            buffer_ = std::vector<T>(size_power_of_two);
            buffer_index_start_ = 0;
            buffer_index_stop_ = 0;
            items_inside_ = 0;
    
            is_inited_ = true;
            return 0;
        }
    
        size_t Size() {
            return buffer_.size();
        }
    
        size_t Items() {
            return items_inside_;
        }
    
        bool IsFull() {
            return items_inside_ >= buffer_.size();
        }
    
        int Destroy() {
            buffer_.clear();
            buffer_index_start_ = 0;
            buffer_index_stop_ = 0;
        }
    
        int Push(T item) {
            if (!is_inited_) return -1;
            if (items_inside_ >= buffer_.size()) return -1;
    
            buffer_[buffer_index_stop_] = item;
            buffer_index_stop_ = (buffer_index_stop_ + 1) & modulo_mask;
            items_inside_++;
    
            return 0;
        }
    
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
    };

}
}

#endif