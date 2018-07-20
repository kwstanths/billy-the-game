#include "RNGenerator.hpp"

#include <cstdlib>
#include <ctime>

#include <limits>
#include <cstddef>
#include <iostream>

#include "debug_tools/Console.hpp"

#include "HelpFunctions.hpp"

namespace dt = debug_tools;

namespace game_engine {
namespace math {

    RNGenerator::RNGenerator() {

    }

    int RNGenerator::Init(Real_t lcg_seed) {
        
        /* Initialize pseudo random number generator LCG */
        lcg_M_ = std::numeric_limits<std::size_t>::max() - 1;
        lcg_A_ = 1664525;
        lcg_C_ = 1;
        lcg_Z_ = static_cast<size_t>(floor(lcg_seed * lcg_M_));

        is_inited_ = true;
        return 0;
    }

    int RNGenerator::Destroy() {

        is_inited_ = false;
        return 0;
    }

    bool RNGenerator::IsInited() {
        return is_inited_;
    }

    int RNGenerator::GetRand(int min, int max) {
        return rand() % (max - min + 1) + min;
    }

    Real_t RNGenerator::GetRandFloat() {
        return (Real_t)rand() / (Real_t)RAND_MAX;
    }

    Real_t RNGenerator::GetPseudoRandFloat() {
        lcg_Z_ = (lcg_A_ * lcg_Z_ + lcg_C_) % lcg_M_;
        return ((Real_t)lcg_Z_ / ((Real_t)lcg_M_));
    }

    void RNGenerator::GetPerlinNoise1d(size_t n_points, Real_t height, Real_t amplitude, size_t wavelength, std::vector<Real_t>& out) {

        if (out.size() < n_points) {
            dt::Console(dt::CRITICAL, "RNGenerator::GetPerlinNoise1d(): output vector size smaller than n_points");
            return;
        }

        size_t index = 0;
        Real_t h2 = (Real_t)height / 2.0f;
        Real_t value = h2;
        Real_t start = GetPseudoRandFloat() - 0.5f;
        Real_t end = GetPseudoRandFloat() - 0.5f;

        while (index < n_points) {
            if (index % wavelength == 0) {
                start = end;
                end = GetPseudoRandFloat() - 0.5f;
                value = h2 + start * amplitude;
            } else {
                value = h2 + InterpolationCosine(start, end, ((Real_t)(index % wavelength) / (Real_t)wavelength)) * amplitude;
            }
            out[index] = value;
            index += 1;
        }

    }

}
}