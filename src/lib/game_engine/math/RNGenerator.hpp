#ifndef __RNGenerator_hpp__
#define __RNGenerator_hpp__

#include <vector>

#include "Types.hpp"

namespace game_engine {
namespace math {

    /**
        Get random numbersm noise and other from various sources. 
        GetRand() and GetRandFloat() use std rand(), and the seed is set in the GameEngine::Init() for all calls.
        GetPseudoRandFloat() used the seed given in the Init()
        GenPerlinNoise1D() uses GetPseudoRandFloat()
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
            Get an integer random number between two integers
            @param min The lower bound
            @param max The upper bound
            @return Random number between
        */
        static int GetRand(int min, int max);

        /**
            Get a float random number between zero and one
            @return Random between zero and one
        */
        static Real_t GetRandFloat();

        /**
            Get a float random number between zero and one with custom seed from Init()
            @return Random between zero and one
        */
        Real_t GetPseudoRandFloat();

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

        size_t lcg_M_, lcg_A_, lcg_C_, lcg_Z_;

    };

}
}

#endif