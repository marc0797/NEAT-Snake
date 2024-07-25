#include "NEAT/genes.hpp"
#include <iostream>
#include <cassert>

using std::cout, std::endl;

void testNeuronGene() {
    cout << "Testing NeuronGene..." << endl;
    NeuronGene neuron = {0, 0.5, Activation::SIGMOID};
    assert(neuron.neuron_id == 0);
    assert(neuron.bias == 0.5);
    assert(neuron.activation == Activation::SIGMOID);
    cout << "NeuronGene passed!" << endl;
}

void testLinkGene() {
    cout << "Testing LinkGene..." << endl;
    LinkGene link = {{0, 1}, 0.5, true};
    assert(link.link_id.input_id == 0);
    assert(link.link_id.output_id == 1);
    assert(link.weight == 0.5);
    assert(link.is_enabled == true);
    cout << "LinkGene passed!" << endl;
}

void testNeuronMutator() {
    cout << "Testing NeuronMutator..." << endl;
    Config config("config.cfg");
    NeuronMutator mutator(config);
    NeuronGene neuron = mutator.new_neuron();
    assert(neuron.neuron_id == 0);
    cout << "NeuronMutator passed!" << endl;
}

void testLinkMutator() {
    cout << "Testing LinkMutator..." << endl;
    Config config("config.cfg");
    LinkMutator mutator(config);
    LinkGene link = mutator.new_link(0, 1);
    assert(link.link_id.input_id == 0);
    assert(link.link_id.output_id == 1);
    cout << "LinkMutator passed!" << endl;
}

int main() {
    testNeuronGene();
    testLinkGene();
    testNeuronMutator();
    testLinkMutator();
    cout << "All tests passed!" << endl;
    return 0;
}