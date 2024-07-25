// rng.hpp

#ifndef RNG_HPP
#define RNG_HPP

#include <random>

using std::vector;

class RNG {
    public:
        RNG(unsigned seed = std::random_device()());

        int next_int(int max, int min = 0);

        double gaussian(double mean, double std);

        template <typename T>
        T choose(double p, T a, T b);

        template <typename T>
        T& choose_from(vector<T> &vec);

        template <typename T>
        typename vector<T>::iterator choose_random(const vector<T> &vec);

    private: 
        // Mersenne Twister engine
        std::mt19937 gen;
};

#endif // RNG_HPP