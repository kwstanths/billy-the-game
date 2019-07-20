#include "RNG.hpp"

/* 
    Mersenne twister copyright 
*/
/* A C-program for MT19937, with initialization improved 2002/1/26.
  Coded by Takuji Nishimura and Makoto Matsumoto.

  Before using, initialize the state by using init_genrand(seed)
  or init_by_array(init_key, key_length).

  Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.

    3. The names of its contributors may not be used to endorse or promote
       products derived from this software without specific prior written
       permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  Any feedback is very welcome.
  http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
  email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/


/*
    Mother of all copyright
*/
/*
    Article: 16024 of sci.math.num-analysis
    Xref: taurus.cs.nps.navy.mil sci.stat.consult:7790 sci.math.num-analysis:16024
    Path: taurus.cs.nps.navy.mil!lll-winken.llnl.gov!uwm.edu!news.alpha.net!news.mathworks.com!udel!ssnet.com!usenet
    From: Bob Wheeler <bwheeler@ssnet.com>
    Newsgroups: sci.stat.consult,sci.math.num-analysis
    Subject: Marsaglia's Mother of all RNG's (Long?)
    Date: Fri, 28 Oct 94 19:32:08 EDT
    Organization: SSNet -- Public Internet Access in Delaware!
    Lines: 285
    Distribution: inet
    Message-ID: <38s2p1$qaf@marlin.ssnet.com>
    NNTP-Posting-Host: echip.com
    Mime-Version: 1.0
    Content-Type: TEXT/PLAIN; charset=US-ASCII
    X-Newsreader: NEWTNews & Chameleon -- TCP/IP for MS Windows from NetManage


    Several people have asked me to post this:
    First the C program, then George Marsaliga's post with details
    about the RNG.  He claims a period of about 2^250 for this and
    that it passes all of the usual tests.  I've tried it enough
    to be sure his claim is reasonable.

    The program:
*/

/* Mother **************************************************************
|	George Marsaglia's The mother of all random number generators
|		producing uniformly distributed pseudo random 32 bit values
with
|		period about 2^250.
|	The text of Marsaglia's posting is appended at the end of the function.
|
|	The arrays mother1 and mother2 store carry values in their
|		first element, and random 16 bit numbers in elements 1 to 8.
|		These random numbers are moved to elements 2 to 9 and a new
|		carry and number are generated and placed in elements 0 and 1.
|	The arrays mother1 and mother2 are filled with random 16 bit values
|		on first call of Mother by another generator.  mStart is the
switch.
|
|	Returns:
|	A 32 bit random number is obtained by combining the output of the
|		two generators and returned in *pSeed.  It is also scaled by
|		2^32-1 and returned as a double between 0 and 1
|
|	SEED:
|	The inital value of *pSeed may be any long value
|
|	Bob Wheeler 8/8/94
*/

#include <string.h>
#include <stdexcept>

#include "HelpFunctions.hpp"

namespace game_engine {
namespace math {
    

    double RNG::rng_between(double a, double b) {
        return Lerp(a, b, rng());
    }

    CongruentialLinearGenerator::CongruentialLinearGenerator() {
        multiplier_ = 16807;
        increment_ = 0;
        seed_ = 48271;
        modulo_ = static_cast<int>(pow(2, 31) - 1);
    }

    CongruentialLinearGenerator::CongruentialLinearGenerator(Parameterization p) {
    switch (p)
    {
        case CongruentialLinearGenerator::ONE:
            multiplier_ = 16807;
            increment_ = 0;
            seed_ = 3793;
            break;
        case CongruentialLinearGenerator::TWO:
            multiplier_ = 16807;
            increment_ = 0;
            seed_ = 4387;
            break;
        case CongruentialLinearGenerator::THREE:
            multiplier_ = 16807;
            increment_ = 0;
            seed_ = 4587;
            break;
        case CongruentialLinearGenerator::FOUR:
            multiplier_ = 16807;
            increment_ = 0;
            seed_ = 5749;
            break;
        case CongruentialLinearGenerator::FIVE:
            multiplier_ = 16807;
            increment_ = 0;
            seed_ = 48271;
            break;
        default:
            throw std::invalid_argument("Out of range");
        }
    
    }

    double CongruentialLinearGenerator::rng() {
        return (double)(seed_ = (multiplier_ * seed_ + increment_) % modulo_) / modulo_;
    }

    FibonacciLaggedGenerator::FibonacciLaggedGenerator(Parameterization p, size_t size) {
        switch (p)
        {
        case FibonacciLaggedGenerator::ONE:
            j_ = 3;
            k_ = 177;
            m_ = 2147483647;
            break;
        case FibonacciLaggedGenerator::TWO:
            j_ = 12;
            k_ = 600;
            m_ = 2147483647;
            break;
        case FibonacciLaggedGenerator::THREE:
            j_ = 10;
            k_ = 548;
            m_ = 2147483647;
            break;
        case FibonacciLaggedGenerator::FOUR:
            j_ = 4;
            k_ = 400;
            m_ = 2147483647;
            break;
        case FibonacciLaggedGenerator::FIVE:
            j_ = 7;
            k_ = 477;
            m_ = 2147483647;
            break;
        case FibonacciLaggedGenerator::SIX:
            j_ = 30;
            k_ = 1000;
            m_ = 2147483647;
            break;
        case FibonacciLaggedGenerator::SEVEN:
            j_ = 3;
            k_ = 181;
            m_ = 2147483647;
            break;
        default:
            throw std::invalid_argument("Out of range");
        }
    
        antics = std::vector<int>(size);
        ini();
    }

    double FibonacciLaggedGenerator::rng() {
        unsigned int a, b;
    
        a = antics[n - j_];
        b = antics[n - k_];
    
        /* Check n maybe for out of size? */
        antics[n] = ((a*b) % m_);
    
        n++;
        return (double)antics[n - 1] / m_;
    }
    
    void FibonacciLaggedGenerator::ini() {
        for (int i = 0; i < 6000; i++)
            antics[i] = i;
    }
    
    BlumBlumShubGenerator::BlumBlumShubGenerator(Parameterization p) {
        switch (p)
        {
        case BlumBlumShubGenerator::ONE:
            p_ = 15485867;
            q_ = 23878409;
            seed_ = 8353;
            break;
        case BlumBlumShubGenerator::TWO:
            p_ = 31415821;
            q_ = 772531;
            seed_ = 1878892440;
            break;
        case BlumBlumShubGenerator::THREE:
            p_ = 3264011;
            q_ = 472990951;
            seed_ = 131893;
            break;
        case BlumBlumShubGenerator::FOUR:
            p_ = 3263849;
            q_ = 1302498943;
            seed_ = 6367859;
            break;
        case BlumBlumShubGenerator::FIVE:
            p_ = 3264011;
            q_ = 472990951;
            seed_ = 9883;
            break;
        case BlumBlumShubGenerator::SIX:
            p_ = 4990364893;
            q_ = 58105693;
            seed_ = 6681193;
            break;
        case BlumBlumShubGenerator::SEVEN:
            p_ = 20359;
            q_ = 43063;
            seed_ = 1993;
            break;
        case BlumBlumShubGenerator::EIGHT:
            p_ = 87566873;
            q_ = 15485143;
            seed_ = 193945;
            break;
        case BlumBlumShubGenerator::NINE:
            p_ = 87566873;
            q_ = 15485143;
            seed_ = 740191;
            break;
        case BlumBlumShubGenerator::TEN:
            p_ = 87566873;
            q_ = 5631179;
            seed_ = 191;
            break;
        default:
            throw std::invalid_argument("Out of range");
        }
    
        M_ = p_ * q_;
        actual_ = seed_;
    }
    
    double BlumBlumShubGenerator::rng() {
        ll r = fmod(actual_*actual_, M_);
        actual_ = r;
        return r / M_;
    }
    
    BlumBlumShubGenerator::ll BlumBlumShubGenerator::getirandom(int i) {
    
        ll g = gcd(p_, q_);
        ll lcm = p_ * q_ / g;
    
        ll exp = 1;
        for (int j = 1; j <= i; ++j) exp = fmod((exp + exp), lcm);
    
        ll x0 = seed_ * seed_;
        ll r = x0;
    
        for (int j = 2; j <= exp; ++j) r = fmod((r*x0), M_);
    
        return r / M_;
    }
    
    BlumBlumShubGenerator::ll BlumBlumShubGenerator::gcd(ll a, ll b) {
        if (b == 0) return a;
        return gcd(b, fmod(a, b));
    }
    
    MersenneTwisterGenerator::MersenneTwisterGenerator(unsigned long seed) {
        init_genrand(seed);
    }
    
    MersenneTwisterGenerator::MersenneTwisterGenerator(Parameterization p) {
        switch (p)
        {
        case MersenneTwisterGenerator::ONE:
        {
            unsigned long init[25] = {
                0x95f24dab, 0x0b685215, 0xe76ccae7, 0xaf3ec239, 0x715fad23,
                0x24a590ad, 0x69e4b5ef, 0xbf456141, 0x96bc1b7b, 0xa7bdf825,
                0xc1de75b7, 0x8858a9c9, 0x2da87693, 0xb657f9dd, 0xffdc8a9f,
                0x8121da71, 0x8b823ecb, 0x885d05f5, 0x4e20cd47, 0x5a9ad5d9,
                0x512c0c03, 0xea857ccd, 0x4cc1d30f, 0x8891a8a1, 0xa6b7aadb
            };
            init_by_array(init, 25);
            break;
        }
        case MersenneTwisterGenerator::TWO:
        {
            unsigned long init[25] = {
                0xff76afcb, 0x45a85215, 0xfa846afa, 0xaf3ec239, 0x8925adf3,
                0x98acf349, 0x9fb4b97a, 0x96bc1b7b, 0xbc450928, 0x5f89ab5f,
                0xabcdef12, 0xa09580fc, 0xa7bdf825, 0x264870ac, 0xab89c600,
                0x71224a71, 0x9997264a, 0xc1de75b7, 0x9845683b, 0x9bcf45d9,
                0x412c09ff, 0xcc7dea94, 0x65ffcb10, 0xa5459cfb, 0xbc98a6ff
            };
            init_by_array(init, 25);
            break;
        }
        case MersenneTwisterGenerator::THREE:
        {
            unsigned long init[25] = {
                0x631f1690, 0x25973e32, 0xead6f57, 0x6ec9d844, 0x5c49eaee,
                0x64af49b, 0x397c46bc, 0x7e448de9, 0x5a9cc3e5, 0x1afe3625,
                0x3ca88ecf, 0x6ebe4208, 0xc058df5, 0xcbe5be9, 0x3102bbe2,
                0x26a02c5e, 0x541c8153, 0x67906f60, 0x10db9daa, 0x697d2d2,
                0x6d68ab2, 0x3a966cd0, 0x63f37e85, 0x5895f5fa, 0x38a5d054
            };
            init_by_array(init, 25);
            break;
        }
        case MersenneTwisterGenerator::FOUR:
        {
            unsigned long init[25] = {
                0x4ab26e78, 0x21faa2fa, 0x5451cf49, 0x6181ef69, 0x3e6400e6,
                0x14217e23, 0x710757d0, 0x5015cd1a, 0x424479da, 0x1a9a9e69,
                0x475e256a, 0x368db37e, 0x6a3b714c, 0x327b517e, 0x1f461b51,
                0x29bacf25, 0x5d5babb3, 0x51bf6b48, 0x7e0f6384, 0x2b4b8b53,
                0x72e3413a, 0x116ae494, 0x3494b2fb, 0xb13a31, 0x64429599
            };
            init_by_array(init, 25);
            break;
        }
        case MersenneTwisterGenerator::FIVE:
        {
            unsigned long init[25] = {
                0x4353d0cd, 0xb03e0c6, 0x189a769b, 0x54e49eb4, 0x71f32454,
                0x2ca88611, 0x836c40e, 0x2901d82, 0x3a95f874, 0x8138641,
                0x1e7ff521, 0x7c3dbd3d, 0x737b8ddc, 0x6ceaf087, 0x22221a70,
                0x4516dde9, 0x3006c83e, 0x614fd4a1, 0x419ac241, 0x5577f8e1,
                0x440badfc, 0x5072367, 0x3804823e, 0x77465f01, 0x7724c67e
            };
            init_by_array(init, 25);
            break;
        }
        case MersenneTwisterGenerator::SIX:
        {
            unsigned long init[25] = {
                0x5c482a97, 0x2463b9ea, 0x5e884adc, 0x51ead36b, 0x2d517796,
                0x580bd78f, 0x153ea438, 0x3855585c, 0x70a64e2a, 0x6a2342ec,
                0x2a487cb0, 0x1d4ed43b, 0x725a06fb, 0x2cd89a32, 0x57e4ccaf,
                0x7a6d8d3c, 0x4b588f54, 0x542289ec, 0x6de91b18, 0x38437fdb,
                0x7644a45c, 0x32fff902, 0x684a481a, 0x579478fe, 0x749abb43
            };
            init_by_array(init, 25);
            break;
        }
        case MersenneTwisterGenerator::SEVEN:
        {
            unsigned long init[25] = {
                0x77ae35eb, 0x579be4f1, 0x310c50b3, 0x5ff87e05, 0x2f305def,
                0x25a70bf7, 0x1dbabf00, 0x4ad084e9, 0x1f48eaa1, 0x1381823a,
                0x5db70ae5, 0x100f8fca, 0x6590700b, 0x15014acb, 0x5f5e7fd0,
                0x98a3148, 0x799d0247, 0x6b94764, 0x42c296bd, 0x168e121f,
                0x1eba5d23, 0x661e3f1e, 0x5dc79ea8, 0x540a471c, 0x7bd3ee7b
            };
            init_by_array(init, 25);
            break;
        }
        default:
            throw std::invalid_argument("Out of range");
        }
    }

    unsigned long MersenneTwisterGenerator::genrand_int32(void) {
        unsigned long y;
        static unsigned long mag01[2] = { 0x0UL, MATRIX_A };
        /* mag01[x] = x * MATRIX_A  for x=0,1 */
    
        if (mti >= N) { /* generate N words at one time */
            int kk;
    
            if (mti == N + 1)   /* if init_genrand() has not been called, */
                init_genrand(5489UL); /* a default initial seed is used */
    
            for (kk = 0; kk < N - M; kk++) {
                y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
                mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1UL];
            }
            for (; kk < N - 1; kk++) {
                y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
                mt[kk] = mt[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
            }
            y = (mt[N - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
            mt[N - 1] = mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];
    
            mti = 0;
        }
    
        y = mt[mti++];
    
        /* Tempering */
        y ^= (y >> 11);
        y ^= (y << 7) & 0x9d2c5680UL;
        y ^= (y << 15) & 0xefc60000UL;
        y ^= (y >> 18);
    
        return y;
    }
    
    long MersenneTwisterGenerator::genrand_int31(void) {
        return (long)(genrand_int32() >> 1);
    }
    
    double MersenneTwisterGenerator::genrand_real1(void) {
        return genrand_int32()*(1.0 / 4294967295.0);
        /* divided by 2^32-1 */
    }
    
    double MersenneTwisterGenerator::genrand_real2(void) {
        return genrand_int32()*(1.0 / 4294967296.0);
        /* divided by 2^32 */
    }
    
    double MersenneTwisterGenerator::genrand_real3(void) {
        return (((double)genrand_int32()) + 0.5)*(1.0 / 4294967296.0);
        /* divided by 2^32 */
    }

    double MersenneTwisterGenerator::genrand_res53(void) {
        /* These real versions are due to Isaku Wada, 2002/01/09 added */
        unsigned long a = genrand_int32() >> 5, b = genrand_int32() >> 6;
        return(a*67108864.0 + b)*(1.0 / 9007199254740992.0);
    }
    
    double MersenneTwisterGenerator::rng() {
        return genrand_real1();
    }
    
    void MersenneTwisterGenerator::init_genrand(unsigned long s) {
        mt[0] = s & 0xffffffffUL;
        for (mti = 1; mti < N; mti++) {
            mt[mti] = (1812433253UL * (mt[mti - 1] ^ (mt[mti - 1] >> 30)) + mti);
            /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
            /* In the previous versions, MSBs of the seed affect   */
            /* only MSBs of the array mt[].                        */
            /* 2002/01/09 modified by Makoto Matsumoto             */
            mt[mti] &= 0xffffffffUL;
            /* for >32 bit machines */
        }
    }
    
    void MersenneTwisterGenerator::init_by_array(unsigned long init_key[], int key_length) {
        int i, j, k;
        init_genrand(19650218UL);
        i = 1; j = 0;
        k = (N > key_length ? N : key_length);
        for (; k; k--) {
            mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1664525UL))
                + init_key[j] + j; /* non linear */
            mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
            i++; j++;
            if (i >= N) { mt[0] = mt[N - 1]; i = 1; }
            if (j >= key_length) j = 0;
        }
        for (k = N - 1; k; k--) {
            mt[i] = (mt[i] ^ ((mt[i - 1] ^ (mt[i - 1] >> 30)) * 1566083941UL))
                - i; /* non linear */
            mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
            i++;
            if (i >= N) { mt[0] = mt[N - 1]; i = 1; }
        }
        mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */
    }
    
    MotherOfAllGenerator::MotherOfAllGenerator(Parameterization p) {
        switch (p)
        {
        case MotherOfAllGenerator::ONE:
            seed_ = 0x3ab6016a;
            break;
        case MotherOfAllGenerator::TWO:
            seed_ = 0x0214361;
            break;
        case MotherOfAllGenerator::THREE:
            seed_ = 0x2d3cde4;
            break;
        case MotherOfAllGenerator::FOUR:
            seed_ = 0x5367f87;
            break;
        case MotherOfAllGenerator::FIVE:
            seed_ = 0x3d46c4d;
            break;
        case MotherOfAllGenerator::SIX:
            seed_ = 0x06fef69d;
            break;
        case MotherOfAllGenerator::SEVEN:
            seed_ = 0x220f9360;
            break;
        case MotherOfAllGenerator::EIGHT:
            seed_ = 0x30beaf03;
            break;
        case MotherOfAllGenerator::NINE:
            seed_ = 0x3a5391ec;
            break;
        case MotherOfAllGenerator::TEN:
            seed_ = 0x15693cd3;
            break;
        default:
            throw std::invalid_argument("Out of range");
            break;
        }
    }
    
    double MotherOfAllGenerator::rng() {
        unsigned long  number, number1, number2;
        short n, *p;
        unsigned short sNumber;
    
        /* Initialize motheri with 9 random values the first time */
        if (mStart) {
            sNumber = seed_ & m16Mask;   /* The low 16 bits */
            number = seed_ & m31Mask;   /* Only want 31 bits */
    
            p = mother1;
            for (n = 18; n--;) {
                number = 30903 * sNumber + (number >> 16);   /* One line multiply-with-cary */
                *p++ = sNumber = number & m16Mask;
                if (n == 9) p = mother2;
            }
    
            /* make cary 15 bits */
            mother1[0] &= m15Mask;
            mother2[0] &= m15Mask;
            mStart = 0;
        }
    
        /* Move elements 1 to 8 to 2 to 9 */
        memmove(mother1 + 2, mother1 + 1, 8 * sizeof(short));
        memmove(mother2 + 2, mother2 + 1, 8 * sizeof(short));
    
        /* Put the carry values in numberi */
        number1 = mother1[0];
        number2 = mother2[0];
    
        /* Form the linear combinations */
    
        number1 += 1941 * mother1[2] + 1860 * mother1[3] + 1812 * mother1[4] + 1776 * mother1[5] +
            1492 * mother1[6] + 1215 * mother1[7] + 1066 * mother1[8] + 12013 * mother1[9];
    
        number2 += 1111 * mother2[2] + 2222 * mother2[3] + 3333 * mother2[4] + 4444 * mother2[5] +
            5555 * mother2[6] + 6666 * mother2[7] + 7777 * mother2[8] + 9272 * mother2[9];
    
        /* Save the high bits of numberi as the new carry */
        mother1[0] = number1 / m16Long;
        mother2[0] = number2 / m16Long;
        /* Put the low bits of numberi into motheri[1] */
        mother1[1] = m16Mask & number1;
        mother2[1] = m16Mask & number2;
    
        /* Combine the two 16 bit random numbers into one 32 bit */
        seed_ = (((long)mother1[1]) << 16) + (long)mother2[1];
    
        /* Return a double value between 0 and 1 */
        return ((double)seed_) / m32Double;
    }
    

/*
    Mother of all generators, comments from the creators
*/
/*
*********************
Marsaglia's comments

         Yet another RNG
Random number generators are frequently posted on
the network; my colleagues and I posted ULTRA in
1992 and, from the number of requests for releases
to use it in software packages, it seems to be
widely used.

I have long been interested in RNG's and several
of my early ones are used as system generators or
in statistical packages.

So why another one?  And why here?

Because I want to describe a generator, or
rather, a class of generators, so promising
I am inclined to call it

    The Mother of All Random Number Generators

and because the generator seems promising enough
to justify shortcutting the many months, even
years, before new developments are widely
known through publication in a journal.

This new class leads to simple, fast programs that
produce sequences with very long periods.  They
use multiplication, which experience has shown
does a better job of mixing bits than do +,- or
exclusive-or, and they do it with easily-
implemented arithmetic modulo a power of 2, unlike
arithmetic modulo a prime.  The latter, while
satisfactory, is difficult to implement.  But the
arithmetic here modulo 2^16 or 2^32 does not suffer
the flaws of ordinary congruential generators for
those moduli: trailing bits too regular.  On the
contrary, all bits of the integers produced by
this new method, whether leading or trailing, have
passed extensive tests of randomness.

Here is an idea of how it works, using, say, integers
of six decimal digits from which we return random 3-
digit integers.  Start with n=123456, the seed.

Then form a new n=672*456+123=306555 and return 555.
Then form a new n=672*555+306=373266 and return 266.
Then form a new n=672*266+373=179125 and return 125,

and so on.  Got it?  This is a multiply-with-carry
sequence x(n)=672*x(n-1)+ carry mod b=1000, where
the carry is the number of b's dropped in the
modular reduction. The resulting sequence of 3-
digit x's has period 335,999.  Try it.

No big deal, but that's just an example to give
the idea. Now consider the sequence of 16-bit
integers produced by the two C statements:

k=30903*(k&65535)+(k>>16); return(k&65535);

Notice that it is doing just what we did in the
example: multiply the bottom half (by 30903,
carefully chosen), add the top half and return the
new bottom.

That will produce a sequence of 16-bit integers
with period > 2^29, and if we concatenate two
such:
      k=30903*(k&65535)+(k>>16);
      j=18000*(j&65535)+(j>>16);
      return((k<<16)+j);
we get a sequence of more than 2^59 32-bit integers
before cycling.

The following segment in a (properly initialized)
C procedure will generate more than 2^118
32-bit random integers from six random seed values
i,j,k,l,m,n:
          k=30903*(k&65535)+(k>>16);
          j=18000*(j&65535)+(j>>16);
          i=29013*(i&65535)+(i>>16);
          l=30345*(l&65535)+(l>>16);
          m=30903*(m&65535)+(m>>16);
          n=31083*(n&65535)+(n>>16);
          return((k+i+m)>>16)+j+l+n);

And it will do it much faster than any of several
widely used generators designed to use 16-bit
integer arithmetic, such as that of Wichman-Hill
that combines congruential sequences for three
15-bit primes (Applied Statistics, v31, p188-190,
1982), period about 2^42.

I call these multiply-with-carry generators. Here
is an extravagant 16-bit example that is easily
implemented in C or Fortran. It does such a
thorough job of mixing the bits of the previous
eight values that it is difficult to imagine a
test of randomness it could not pass:

x[n]=12013x[n-8]+1066x[n-7]+1215x[n-6]+1492x[n-5]+1776x[n-4]
 +1812x[n-3]+1860x[n-2]+1941x[n-1]+carry mod 2^16.

The linear combination occupies at most 31 bits of
a 32-bit integer. The bottom 16 is the output, the
top 15 the next carry. It is probably best to
implement with 8 case segments. It takes 8
microseconds on my PC. Of course it just provides
16-bit random integers, but awfully good ones. For
32 bits you would have to combine it with another,
such as

x[n]=9272x[n-8]+7777x[n-7]+6666x[n-6]+5555x[n-5]+4444x[n-4]
     +3333x[n-3]+2222x[n-2]+1111x[n-1]+carry mod 2^16.

Concatenating those two gives a sequence of 32-bit
random integers (from 16 random 16-bit seeds),
period about 2^250. It is so awesome it may merit
the Mother of All RNG's title.

The coefficients in those two linear combinations
suggest that it is easy to get long-period
sequences, and that is true.  The result is due to
Cemal Kac, who extended the theory we gave for
add-with-carry sequences: Choose a base b and give
r seed values x[1],...,x[r] and an initial 'carry'
c. Then the multiply-with-carry sequence

 x[n]=a1*x[n-1]+a2*x[n-2]+...+ar*x[n-r]+carry mod b,

where the new carry is the number of b's dropped
in the modular reduction, will have period the
order of b in the group of residues relatively
prime to m=ar*b^r+...+a1b^1-1.  Furthermore, the
x's are, in reverse order, the digits in the
expansion of k/m to the base b, for some 0<k<m.

In practice b=2^16 or b=2^32 allows the new
integer and the new carry to be the bottom and top
half of a 32- or 64-bit linear combination of  16-
or 32-bit integers.  And it is easy to find
suitable m's if you have a primality test:  just
search through candidate coefficients until you
get an m that is a safeprime---both m and (m-1)/2
are prime.  Then the period of the multiply-with-
carry sequence will be the prime (m-1)/2. (It
can't be m-1 because b=2^16 or 2^32 is a square.)

Here is an interesting simple MWC generator with
period> 2^92, for 32-bit arithmetic:

x[n]=1111111464*(x[n-1]+x[n-2]) + carry mod 2^32.

Suppose you have functions, say top() and bot(),
that give the top and bottom halves of a 64-bit
result.  Then, with initial 32-bit x, y and carry
c,  simple statements such as
      y=bot(1111111464*(x+y)+c)
      x=y
      c=top(y)
will, repeated, give over 2^92 random 32-bit y's.

Not many machines have 64 bit integers yet.  But
most assemblers for modern CPU's permit access to
the top and bottom halves of a 64-bit product.

I don't know how to readily access the top half of
a 64-bit product in C.  Can anyone suggest how it
might be done? (in integer arithmetic)

George Marsaglia geo@stat.fsu.edu

*/


}
}