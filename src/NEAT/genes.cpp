// neuron_gene.cpp

#include "NEAT/genes.hpp"
#include "NEAT/rng.hpp"
#include <cassert>


NeuronMutator::NeuronMutator(Config &config) : index(0) {
    std::string section = "DefaultGenome";
    activation = (Activation) config.getInt(
        section,
        "activation",
        (int) Activation::SIGMOID);
    mean = config.getDouble(
        section,
        "bias_init_mean",
        0.0);
    std = config.getDouble(
        section,
        "bias_init_stddev",
        1.0);
    min = config.getDouble(
        section,
        "bias_min",
        -30.0);
    max = config.getDouble(
        section,
        "bias_max",
        30.0);
}

NeuronGene NeuronMutator::new_neuron() {
    // Random bias in Gaussian distribution
    double bias = clamp(new_value(mean, std), min, max);

    return {index++, bias, activation};
}

LinkMutator::LinkMutator(Config &config) : index(0) {
    std::string section = "DefaultGenome";
    mean = config.getDouble(
        section,
        "weight_init_mean",
        0.0);
    std = config.getDouble(
        section,
        "weight_init_stddev",
        1.0);
    min = config.getDouble(
        section,
        "weight_min",
        -30.0);
    max = config.getDouble(
        section,
        "weight_max",
        30.0);
}

LinkGene LinkMutator::new_link(int input_id, int output_id) {
    // Random weight in Gaussian distribution
    double weight = clamp(new_value(mean, std), min, max);

    return {{input_id, output_id}, weight, true};
}

double new_value(double mean, double std) {
    return RNG{}.gaussian(mean, std);
}

double clamp(double value, double min, double max) {
    return std::max(min, std::min(max, value));
}

/**
 * Crossover two neuron genes.
 * 
 * @param n1 The first neuron gene.
 * @param n2 The second neuron gene.
 * @return The offspring neuron gene.
 */
NeuronGene crossover_neuron(const NeuronGene &n1, const NeuronGene &n2) {
    assert(n1.neuron_id == n2.neuron_id);
    
    // Randomly choose bias from either parent
    RNG rng;
    int neuron_id = n1.neuron_id;
    double bias = rng.choose<double>(0.5, n1.bias, n2.bias);
    Activation activation = rng.choose<Activation>(0.5, n1.activation, n2.activation);

    return {neuron_id, bias, activation};
}

/**
 * Crossover two link genes.
 * 
 * @param l1 The first link gene.
 * @param l2 The second link gene.
 * @return The offspring link gene.
 */
LinkGene crossover_link(const LinkGene &l1, const LinkGene &l2) {
    assert(l1.link_id == l2.link_id);

    // Randomly choose weight from either parent
    RNG rng;
    LinkId link_id = l1.link_id;
    double weight = rng.choose<double>(0.5, l1.weight, l2.weight);
    bool is_enabled = rng.choose<bool>(0.5, l1.is_enabled, l2.is_enabled);

    return {link_id, weight, is_enabled};
}