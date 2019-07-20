#ifndef __UniformGrid_hpp__
#define __UniformGrid_hpp__

#include <iostream>
#include <stdexcept>
#include <vector>
#include <math.h>

namespace mth = game_engine::math;
namespace ms = game_engine::memory;
namespace dt = debug_tools;

namespace game_engine {
namespace utility {

    template<typename T, size_t D>
    class UniformGrid {
    public:
        UniformGrid(std::vector<size_t> dims) {
            
            if (dims.size() != D) {
                std::string error("Wrong number of arguments");
                throw std::invalid_argument(error);
            }
            dims_ = dims;
            
            size_t total_size = 1;
            for (size_t i = 0; i < dims.size(); i++) total_size *= dims[i];

            array_ = std::vector<T>(static_cast<size_t>(total_size));
        }
    
        ~UniformGrid() {
            array_.clear();
        }
    
        template<typename I, typename ... Args>
        T& at(I first, Args ... rest) {
            const int size = sizeof...(rest) + 1;
            if (size != D) {
                std::string error("Wrong number of arguments");
                throw std::invalid_argument(error);
            }
    
            size_t index = 0;
            unpack(index, 0, first, rest...);

            return array_[index];
        }
    
    private:
        std::vector<T> array_;
        std::vector<size_t> dims_;

        template<typename I>
        size_t unpack(size_t& index, size_t level, I last) {
            index += last;
            return dims_[level];
        }
    
        template<typename I, typename ... Args>
        size_t unpack(size_t& index, size_t level, I first, Args ... rest) {

            size_t ret = unpack(index, level + 1, rest...);
            index += first * ret;

            return  ret * dims_[level];
        }
    
    };

}
}

#endif