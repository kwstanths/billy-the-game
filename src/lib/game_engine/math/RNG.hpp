#ifndef __RNG_hpp__
#define __RNG_hpp__

#include <vector>

namespace game_engine {
namespace math {

    class RNG {
    public:
        virtual double rng() = 0;
        double rng_between(double a, double b);
    };
    
    class CongruentialLinearGenerator : public RNG {
    public:
        enum Parameterization {
            ONE,
            TWO,
            THREE,
            FOUR,
            FIVE,
        };
    
        CongruentialLinearGenerator();

        CongruentialLinearGenerator(Parameterization p);
    
        double rng();
    
    private:
        int multiplier_, increment_;
        unsigned int modulo_, seed_;
    };
    
    class FibonacciLaggedGenerator : public RNG {
    public:
        enum Parameterization {
            ONE,
            TWO,
            THREE,
            FOUR,
            FIVE,
            SIX,
            SEVEN,
        };
    
        /**
            @param size Size needs to be atleast 3000 plus the number of random numbers requested
        */
        FibonacciLaggedGenerator(Parameterization p, size_t size);
    
        double rng();
    
    private:
        int n = 3000;
        std::vector<int> antics;
        int j_, k_, m_;
    
        void ini();
    };
    
    class BlumBlumShubGenerator : public RNG {
    private:
        typedef long double ll;
    
    public:
        enum Parameterization {
            ONE,
            TWO,
            THREE,
            FOUR,
            FIVE,
            SIX,
            SEVEN,
            EIGHT,
            NINE,
            TEN,
        };
    
        BlumBlumShubGenerator(Parameterization p);
    
        double rng();
    
        ll getirandom(int i);
    
    private:
        ll p_, q_, M_, seed_, actual_;
    
        ll gcd(ll a, ll b);
    };
    
    class MersenneTwisterGenerator : public RNG {
    private:
        /* Period parameters */
        static const int N = 624;
        static const int M = 397;
        static const unsigned long MATRIX_A = 0x9908b0dfUL;     /* constant vector a */
        static const unsigned long UPPER_MASK = 0x80000000UL;   /* most significant w-r bits */
        static const unsigned long LOWER_MASK = 0x7fffffffUL;   /* least significant r bits */
    
    public:
        enum Parameterization {
            ONE,
            TWO,
            THREE,
            FOUR,
            FIVE,
            SIX,
            SEVEN,
        };
    
        /**
            Initialize with time
        */
        MersenneTwisterGenerator();

        /**
            Initialize with seed
        */
        MersenneTwisterGenerator(unsigned long seed);
    
        /**
            Use custom set initialization parameters
        */
        MersenneTwisterGenerator(Parameterization p);
    
        /**
            Generates a random number on [0,0xffffffff]-interval
        */
        unsigned long genrand_int32(void);
    
        /**
            Generates a random number on [0,0x7fffffff]-interval
        */
        long genrand_int31(void);
    
        /**
            Generates a random number on [0,1]-real-interval
        */
        double genrand_real1(void);
    
        /**
            Generates a random number on [0,1)-real-interval
        */
        double genrand_real2(void);
    
        /**
            Generates a random number on (0,1)-real-interval
        */
        double genrand_real3(void);
    
        /**
            Generates a random number on [0,1) with 53-bit resolution
        */
        double genrand_res53(void);
    
        /**
            Generate a random number in the [0,1]-real-interval
        */
        double rng();
    
    private:
        unsigned long mt[N];    /* the array for the state vector */
        int mti = N + 1;        /* mti==N+1 means mt[N] is not initialized */
    
    
        /* initializes mt[N] with a seed */
        void init_genrand(unsigned long s);
    
        /* initialize by an array with array-length */
        /* init_key is the array for initializing keys */
        /* key_length is its length */
        /* slight change for C++, 2004/2/26 */
        void init_by_array(unsigned long init_key[], int key_length);
    
    };
    
    class MotherOfAllGenerator : public RNG {
    private:
        long m16Long = 65536L;             /* 2^16 */
        int m16Mask = 0xFFFF;              /* mask for lower 16 bits */
        int m15Mask = 0x7FFF;              /* mask for lower 15 bits */
        int m31Mask = 0x7FFFFFFF;          /* mask for 31 bits */
        double m32Double = 4294967295.0;   /* 2^32-1 */
    
    public:
    
        enum Parameterization {
            ONE,
            TWO,
            THREE,
            FOUR,
            FIVE,
            SIX,
            SEVEN,
            EIGHT,
            NINE,
            TEN,
        };
    
        MotherOfAllGenerator(Parameterization p);
    
        /**
            Generate random number [0,1]
        */
        double rng();
    
    private:
        unsigned long seed_;
        short mother1[10];
        short mother2[10];
        short mStart = 1;
    };

}
}

#endif