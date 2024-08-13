// network.hpp

#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "NEAT/activations.hpp"
#include "NEAT/genome.hpp"
#include <unordered_map>
#include <unordered_set>
#include <queue>

struct NeuronInput {
    int input_id;
    double weight;
};

struct Neuron {
    int neuron_id;
    double bias;
    Activation activation;
    vector<NeuronInput> inputs;
};

class FeedForwardNeuralNetwork {
    public:
        FeedForwardNeuralNetwork(
                vector<int> input_ids, 
                vector<int> output_ids, 
                vector<Neuron> neurons) : 
                input_ids(input_ids), 
                output_ids(output_ids), 
                neurons(neurons) {};
        vector<double> activate(const vector<double> &inputs);

    private:
        vector<int> input_ids;
        vector<int> output_ids;
        vector<Neuron> neurons;
};

FeedForwardNeuralNetwork create_from_genome(Genome &genome);

vector<vector<int>> create_layers(
    vector<int> input_ids, 
    vector<int> output_ids, 
    const vector<LinkGene> &links);

#endif // NETWORK_HPP