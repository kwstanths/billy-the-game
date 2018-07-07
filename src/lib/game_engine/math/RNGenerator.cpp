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

    int RNGenerator::Init(float lcg_seed) {
        
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

    float RNGenerator::GetRandFloat() {
        return (float)rand() / (float)RAND_MAX;
    }

    float RNGenerator::GetPseudoRandFloat() {
        lcg_Z_ = (lcg_A_ * lcg_Z_ + lcg_C_) % lcg_M_;
        return ((float)lcg_Z_ / ((float)lcg_M_));
    }

    void RNGenerator::GetPerlinNoise1d(size_t n_points, float height, float amplitude, size_t wavelength, std::vector<float>& out) {

        if (out.size() < n_points) {
            dt::Console(dt::CRITICAL, "RNGenerator::GetPerlinNoise1d(): output vector size smaller than n_points");
            return;
        }

        size_t index = 0;
        float h2 = (float)height / 2.0f;
        float value = h2;
        float start = GetPseudoRandFloat() - 0.5f;
        float end = GetPseudoRandFloat() - 0.5f;

        while (index < n_points) {
            if (index % wavelength == 0) {
                start = end;
                end = GetPseudoRandFloat() - 0.5f;
                value = h2 + start * amplitude;
            } else {
                value = h2 + InterpolationCosine(start, end, ((float)(index % wavelength) / (float)wavelength)) * amplitude;
            }
            out[index] = value;
            index += 1;
        }

    }

}
}