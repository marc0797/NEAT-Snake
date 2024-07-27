// rng.hpp

#ifndef RNG_HPP
#define RNG_HPP

#include <random>

using std::vector;

class RNG {
    public:
        RNG(unsigned seed = std::random_device()());

        int next_int(int max, int min = 0);

        double uniform(double min = 0.0, double max = 1.0);

        double gaussian(double mean, double std);

        template <typename T>
        T choose(double p, const T& a, const T& b) {
            std::bernoulli_distribution dist(p);
            return dist(gen) ? a : b;
        }

        template <typename T>
        T& choose_from(vector<T> &vec) {
            return vec[next_int(vec.size() - 1)];
        }

        template <typename T>
        typename vector<T>::iterator choose_random(vector<T> &vec) {
            return vec.begin() + next_int(vec.size() - 1);
        }

    private: 
        // Mersenne Twister engine
        std::mt19937 gen;
};

#endif // RNG_HPP