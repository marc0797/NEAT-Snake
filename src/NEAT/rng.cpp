// rng.cpp

#include "NEAT/rng.hpp"

RNG::RNG(unsigned seed) : gen(seed) {}

/**
 * Generates a random integer between min and max.
 * 
 * @param max The maximum value of the random integer.
 * @param min The minimum value of the random integer. DEFAULT 0.
 * @return A random integer between min and max.
 */
int RNG::next_int(int max, int min) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

/**
 * Generates a random double from a Gaussian distribution.
 * 
 * @param mean The mean of the Gaussian distribution.
 * @param std The standard deviation of the Gaussian distribution.
 * @return A random double from a Gaussian distribution.
 */
double RNG::gaussian(double mean, double std) {
    std::normal_distribution<double> dist(mean, std);
    return dist(gen);
}

/**
 * Chooses between two values based on a probability.
 * 
 * @param p The probability of choosing a.
 * @param a The first value.
 * @param b The second value.
 * @return a if the random number is less than p, b otherwise.
 */
// template <typename T>
// T RNG::choose(double p, const T& a, const T& b) {
//     std::bernoulli_distribution dist(p);
//     return dist(gen) ? a : b;
// }

/**
 * Chooses a random element from a vector.
 * 
 * @param vec The vector to choose from.
 * @return A reference to a random element in the vector.
 */
// template <typename T>
// T& RNG::choose_from(vector<T> &vec) {
//     return vec[next_int(vec.size() - 1)];
// }

/**
 * Chooses a random iterator from a vector.
 * 
 * @param vec The vector to choose from.
 * @return An iterator to a random element in the vector.
 */
// template <typename T>
// typename vector<T>::iterator RNG::choose_random(const vector<T> &vec) {
//     return vec.begin() + next_int(vec.size() - 1);
// }