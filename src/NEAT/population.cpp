// population.cp

#include "NEAT/population.hpp"
#include <algorithm>

/**
 * Create a new population.
 * 
 * @param config The configuration for the genetic algorithm.
 * @param rng The random number generator to use.
 * @return A new population.
 */
Population::Population(Config &config, RNG &rng) : _config(config), _rng(rng) {
    // Create the initial population
    for (int i = 0; i < _config.population_size(); i++) {
        Genome genome(indexer.next(), _config);
        genome.config_new(_config);
        _genomes.push_back(genome);
    }
}

/**
 * Load a population from a file.
 * 
 * @param filename The name of the file to load the population from.
 * @return A new population.
 */
Population::Population(const string &filename) :
    _config(Config("config_" + filename)) {
    _rng = RNG();
    // Load the population from the file
    ifstream file(filename);

    // Initialize the GenomeIndexer and genomes
    indexer = GenomeIndexer();

    int genome_count;
    file >> genome_count;
    for (int i = 0; i < genome_count; i++) {
        indexer.next();
        Genome genome;
        file >> genome;
        _genomes.push_back(genome);
    }
}

/**
 * Run the genetic algorithm for a given number of generations.
 * 
 * @param compute_fitness The fitness function to use.
 * @param max_generations The maximum number of generations to run.
 */
// template <typename FitnessFunction>
// void Population::run(FitnessFunction compute_fitness, int max_generations) {
//     // In each generation, calculate the fitness of each genome and
//     // reproduce the next generation
//     for (int i = 0; i < max_generations; i++) {
//         compute_fitness(_genomes.begin(), _genomes.end());
//         update_best();
//         _genomes = reproduce();
//     }
// }

/**
 * Reproduce the next generation of genomes.
 * 
 * @return The next generation of genomes.
 */
vector<Genome> Population::reproduce() {
    // Sort the genomes by fitness
    auto old_genomes = sort_by_fitness(_genomes);
    int cutoff = std::ceil(
        _config.survival_threshold() * old_genomes.size());

    // Keep the top genomes
    vector<Genome> top_genomes(old_genomes.begin(), old_genomes.begin() + cutoff);
    int spawn_size = _config.population_size();
    
    // Create the new population as an empty vector
    vector<Genome> new_generation = {};
    while (spawn_size-- >= 0) {
        // Select two parents at random
        const auto& p1 = _rng.choose_from(top_genomes);
        const auto& p2 = _rng.choose_from(top_genomes);
        Genome offspring = crossover(p1, p2, _config, indexer);
        offspring.mutate(_config);
        new_generation.push_back(offspring);
    }
    return new_generation;
}

/**
 * Update the best genome in the population.
 * 
 * The best genome is the one with the highest fitness.
 */
void Population::update_best() {
    for (const auto &genome : _genomes) {
        if (genome.fitness() > best.fitness()) {
            best = genome;
        }
    }
}

/**
 * Sort the genomes by fitness.
 * 
 * @param genomes The genomes to sort.
 * @return The sorted genomes.
 */
vector<Genome> Population::sort_by_fitness(vector<Genome> &genomes) {
    std::sort(genomes.begin(), genomes.end(), [](const Genome &a, const Genome &b) {
        return a.fitness() > b.fitness();
    });
    return genomes;
}

/**
 * Save the population to a file.
 * 
 * @param filename The name of the file to save the population to.
 * @return true if the population was successfully saved, false otherwise.
 */
bool Population::save_file(const string &filename) const {
    // Open the file
    ofstream file(filename);
    if (!file.is_open()) {
        // If the file could not be opened, print an error message and return false
        cerr << "Could not open file: " << filename << endl;
        return false;
    }

    // Save the configuration to the file
    _config.save("config_" + filename);

    // Write the genomes to the file
    file << _genomes.size() << endl;
    for (const auto &genome : _genomes) {
        file << genome;
    }

    file.close();

    return true;
}