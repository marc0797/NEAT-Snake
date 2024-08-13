// population.hpp

#ifndef NEAT_POPULATION_HPP
#define NEAT_POPULATION_HPP

#include "NEAT/config.hpp"
#include "NEAT/genome.hpp"
#include "NEAT/species.hpp"
#include "rng.hpp"
#include <chrono>

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
        Population(const string &filename);

        template <typename FitnessFunction>
        void run(FitnessFunction compute_fitness, int max_generations) {
            // In each generation, calculate the fitness of each genome and
            // reproduce the next generation
            for (int i = 0; i < max_generations; i++) {
                // Start measuring time
                auto start = std::chrono::high_resolution_clock::now();
                cout << "====== Running generation " << i+1 << " ======" << endl;
                cout << "Population of " << _genomes.size() << 
                    " members in " << "species." << endl;
                compute_fitness(_genomes.begin(), _genomes.end());
                update_best();
                cout << "\t ID \tage\tsize\tfitness\tadj fit\tstag" << endl;
                cout << "\t====\t====\t====\t=======\t=======\t====" << endl;
                for (auto &species : _species) {
                    species.adjust_fitness();
                    float prev_fitness = species.best().fitness();
                    species.update_best();
                    if (species.best().fitness() > prev_fitness) {
                        species._stagnation = 0;
                    } else {
                        species._stagnation++;
                    }

                    cout << "\t" << species.id();
                    cout << "\t" << species._age++;
                    cout << "\t" << species.members().size();
                    cout << "\t" << species.best().fitness();
                    cout << "\t" << species.adjusted_fitness();
                    cout << "\t" << species._stagnation << endl;
                }
                _genomes = reproduce();

                // End measuring time
                auto end = std::chrono::high_resolution_clock::now();
                cout << "\nGeneration time: " << 
                    std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()/1000.0
                    << " seconds\n" << endl;
            }
        }
        vector<Genome> reproduce();

        const Genome &best_genome() const { return best; }

        bool save_file(const string &filename) const;

    private:
        Config _config;
        RNG _rng;
        GenomeIndexer indexer;
        Genome best = Genome(-1, _config);
        vector<Genome> _genomes;
        vector<Species> _species;
        
        void update_best();
        vector<Genome> sort_by_fitness(vector<Genome> &genomes);
};

#endif // NEAT_POPULATION_HPP