#ifndef __UniformGrid_hpp__
#define __UniformGrid_hpp__

#include <iostream>
#include <stdexcept>
#include <vector>
#include <math.h>

namespace game_engine { namespace utility {

    /* A Uniform grid of template type of Data to store T, and template Dimension D */
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
    
        /* Access an element, give a number of indices equal to the dimension D */
        template<typename ... Args>
        T& at(size_t first, Args ... rest) {
            const int size = sizeof...(rest) + 1;
            if (size != D) {
                std::string error("Wrong number of arguments");
                throw std::invalid_argument(error);
            }
    
            size_t index = 0;
            unpack(index, 0, first, rest...);
    
            return array_[index];
        }
    
        std::vector<size_t> GetDimensions() {
            return dims_;
        }
    
    private:
        std::vector<T> array_;
        std::vector<size_t> dims_;
    
        size_t unpack(size_t& index, size_t level, size_t last) {
            index += last;
            return dims_[level];
        }
    
        template<typename ... Args>
        size_t unpack(size_t& index, size_t level, size_t first, Args ... rest) {
    
            size_t ret = unpack(index, level + 1, rest...);
            index += first * ret;
    
            return  ret * dims_[level];
        }
    
    };

}
}

#endif