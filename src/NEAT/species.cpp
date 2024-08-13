// species.cpp

#include "NEAT/species.hpp"

/**
 * Calculate the compatibility between two genomes.
 * 
 * @param g1 The first genome.
 * @param g2 The second genome.
 * @param config The configuration for the genetic algorithm.
 * @return The compatibility between the two genomes.
 */
bool is_compatible(const Genome &g1, const Genome &g2, const Config &config) {
    // Calculate the number of excess and disjoint genes
    int excess_disjoint = 0;
    int matching = 0;
    float weight_diff = 0.0;
    int number_of_genes = std::max(g1.links().size(), 
        g2.links().size());


    // Get the genes of each genome
    auto it1 = g1.links().begin();
    auto it2 = g2.links().begin();
    while (it1 != g1.links().end() && it2 != g2.links().end()) {
        if (it1->link_id == it2->link_id) {
            // Matching gene
            matching++;
            weight_diff += std::abs(it1->weight - it2->weight);
            it1++;
            it2++;
        } else if (it1->link_id < it2->link_id) {
            // Disjoint gene in genome 1
            excess_disjoint++;
            it1++;
        } else {
            // Disjoint gene in genome 2
            excess_disjoint++;
            it2++;
        }
    }

    // Calculate the average weight difference
    weight_diff /= matching;

    // Calculate the compatibility
    return (config.c1() * excess_disjoint / number_of_genes) +
        (config.c2() * weight_diff) < config.threshold();
}

// species.cpp