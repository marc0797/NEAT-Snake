#include "NEAT/population.hpp"
#include "controller.hpp"
#include <iostream>
#include <cassert>
#include <string>

using std::cout, std::endl;

void compute_fitness(
    vector<Genome>::iterator begin,
    vector<Genome>::iterator end) {
    for (auto it = begin; it != end; it++) {
        update_fitness(*it);
    }
}

int main() {
    Config config("config.cfg");
    config.load();
    RNG rng;
    Population population(config, rng);

    population.run(compute_fitness, config.max_generations());

    cout << "Best genome: " << population.best_genome().genome_id << endl;
    cout << "Fitness: " << population.best_genome().fitness() << endl;

    return 0;
}