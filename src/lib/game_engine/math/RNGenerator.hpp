#ifndef __RNGenerator_hpp__
#define __RNGenerator_hpp__

#include <vector>

#include "Types.hpp"
#include "RNG.hpp"

namespace game_engine {
namespace math {

    /**
        Used to generate 1D Perlin noise. If you want RNG values, look into RNG.hpp
    */
    class RNGenerator {
    public:
        RNGenerator();

        /**
            Initialize the generator
            @param lcg_seed The seed for the lcg random number generator. Use GetPseudoRandFloat() for these random numbers
        */
        int Init(Real_t lcg_seed);

        int Destroy();

        bool IsInited();

        /**
            Generate Perlin noise in a one dimensional array
            @param The number of values to generate
            @param height The half of this value is the base of the generated noise
            @param amplitude The factor that multiplies the random numbers generated added to the base value
            @param wavelength The wavelength between peak points
            @param[out] out The vector with the values. If n_points is greater than the out vector size, zeros are returned
        */
        void GetPerlinNoise1d(size_t n_points, Real_t height, Real_t amplitude, size_t wavelength, std::vector<Real_t> & out);

    private:
        bool is_inited_;

        game_engine::math::CongruentialLinearGenerator rng_;
    };

}
}

#endif