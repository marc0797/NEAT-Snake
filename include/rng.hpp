// rng.hpp

#ifndef RNG_HPP
#define RNG_HPP

#include <random>

// Define the RNG class
class RNG {
    public:
        // Constructor
        RNG(unsigned seed = std::random_device{}()) : engine(seed) {}

        // Returns a random integer in the range [0, max]
        int next_int(int max) {
            std::uniform_int_distribution<int> dist(0, max);
            return dist(engine);
        }

        // Choose between a and b with probability p
        template <typename T>
        T choose(double p, T a, T b) {
            std::bernoulli_distribution dist(p);
            return dist(engine) ? a : b;
        }

    private:
        // Mersenne Twister engine
        std::mt19937 engine;
};

#endif