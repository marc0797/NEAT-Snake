#include "NEAT/population.hpp"
#include <iostream>
#include <cassert>

using std::cout, std::endl;

float testFitness(const Genome& genome) {
    return 0.0f;
}

void testPopulation() {
    Config config("config.cfg");
    RNG rng;
    Population population(config, rng);

    population.run(testFitness, 5);

    cout << "Population tests passed!" << endl;
}

int main() {
    testPopulation();

    return 0;
}