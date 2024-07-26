#include "NEAT/genome.hpp"
#include <iostream>
#include <cassert>

using std::cout, std::endl;

void testGenome() {
    cout << "Testing Genome..." << endl;
    Config config("config.cfg");
    Genome genome(0, config);
    assert((
        genome.num_inputs() +
        genome.num_outputs() +
        genome.num_hidden()) == genome.neurons().size());
    cout << "Genome constructor passed!" << endl;

    genome.add_neuron({100, 0.5, Activation::SIGMOID});
    assert(genome.find_neuron(100).has_value());
    cout << "Genome add_neuron passed!" << endl;

    genome.add_link({{100, 0}, 0.5, true});
    assert(genome.find_link({100, 0}).has_value());
    cout << "Genome add_link passed!" << endl;

    assert(genome.fitness() == FitnessNotCalculated);
    cout << "Genome fitness passed!" << endl;
}

int main() {
    testGenome();
    cout << "All tests passed!" << endl;
    return 0;
}