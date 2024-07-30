// population.hpp

#ifndef NEAT_POPULATION_HPP
#define NEAT_POPULATION_HPP

#include "NEAT/config.hpp"
#include "NEAT/genome.hpp"
#include "rng.hpp"

class Population {
    public:
        /**
         * @brief Construct a new Population object
         * 
         * Procedure:
         * 1. Create a new population of genomes
         * 2. Calculate the fitness of each genome
         * 3. Sort the genomes by fitness
         * 4. Divide the genomes into species
         * 5. Cut the population down to the threshold
         * 6. Breed the new population
         * 7. Repeat from step 2
         * 
         */
        Population(Config &config, RNG &rng);

        template <typename FitnessFunction>
        void run(FitnessFunction compute_fitness, int max_generations) {
            // In each generation, calculate the fitness of each genome and
            // reproduce the next generation
            for (int i = 0; i < max_generations; i++) {
                compute_fitness(_genomes.begin(), _genomes.end());
                update_best();
                _genomes = reproduce();
            }
        }
        vector<Genome> reproduce();

    private:
        Config _config;
        RNG _rng;
        GenomeIndexer indexer;
        Genome best = Genome(-1, _config);
        vector<Genome> _genomes;
        vector<Genome> _species;
        
        void update_best();
        vector<Genome> sort_by_fitness(vector<Genome> &genomes);
};

#endif // NEAT_POPULATION_HPP