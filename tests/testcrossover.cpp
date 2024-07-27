#include "NEAT/genome.hpp"
#include <iostream>
#include <cassert>


void testCrossover() {
    Config config("config.cfg");
    GenomeIndexer indexer;
    Genome genome1(indexer.next(), config);
    Genome genome2(indexer.next(), config);

    genome1.config_new(config);
    genome2.config_new(config);

    genome1.print();
    genome2.print();

    Genome child = crossover(genome1, genome2, config, indexer);

    child.print();

    cout << "Crossover tests passed!" << endl;
}

int main() {
    testCrossover();

    return 0;
}