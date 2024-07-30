#include "NEAT/population.hpp"
#include <iostream>
#include <cassert>

using std::cout, std::endl;

float testFitness(const Genome& genome) {
    return 0.0f;
}

void compute_fitness(
    vector<Genome>::iterator begin,
    vector<Genome>::iterator end) {
    for (auto it = begin; it != end; it++) {
        it->fitness() = testFitness(*it);
    }
}

void testPopulation() {
    Config config("config.cfg");
    RNG rng;
    Population population(config, rng);

    population.run(compute_fitness, 5);

    cout << "Population tests passed!" << endl;
}

int main() {
    testPopulation();

    return 0;
}