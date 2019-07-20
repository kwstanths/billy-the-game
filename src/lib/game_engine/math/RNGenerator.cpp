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

    void RNGenerator::GetPerlinNoise1d(size_t n_points, Real_t height, Real_t amplitude, size_t wavelength, std::vector<Real_t>& out) {

        if (out.size() < n_points) {
            dt::Console(dt::CRITICAL, "RNGenerator::GetPerlinNoise1d(): output vector size smaller than n_points");
            return;
        }

        size_t index = 0;
        Real_t h2 = (Real_t)height / 2.0f;
        Real_t value = h2;
        Real_t start = static_cast<Real_t>(rng_.rng() - 0.5f);
        Real_t end = static_cast<Real_t>(rng_.rng() - 0.5f);

        while (index < n_points) {
            if (index % wavelength == 0) {
                start = end;
                end = static_cast<Real_t>(rng_.rng() - 0.5f);
                value = h2 + start * amplitude;
            } else {
                value = h2 + LerpCosine(start, end, ((Real_t)(index % wavelength) / (Real_t)wavelength)) * amplitude;
            }
            out[index] = value;
            index += 1;
        }

    }

}
}