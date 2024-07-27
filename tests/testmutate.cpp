#include "NEAT/genome.hpp"
#include <iostream>
#include <cassert>

void testMutation() {
    Config config("config.cfg");
    Genome genome(0, config);

    genome.print();

    genome.mutate(config);

    genome.print();

    cout << "Mutation tests passed!" << endl;
}

int main() {
    testMutation();

    return 0;
}