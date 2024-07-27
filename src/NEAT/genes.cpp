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
    mutation_rate = config.getDouble(
        section,
        "mutation_rate",
        0.3);
    mutation_power = config.getDouble(
        section,
        "mutation_power",
        0.8);
    replace_rate = config.getDouble(
        section,
        "replace_rate",
        0.05);
}

NeuronGene NeuronMutator::new_neuron() {
    // Random bias in Gaussian distribution
    double bias = clamp(new_value(mean, std), min, max);

    return {index++, bias, activation};
}

void NeuronMutator::mutate(NeuronGene &neuron, int num_outputs) {
    RNG rng;
    double p = rng.uniform();
    
    // Mutate the bias value
    if (p < replace_rate) {
        neuron.bias = clamp(new_value(mean, std), min, max);
    } else if (p < mutation_rate + replace_rate) {
        double delta = clamp(rng.gaussian(0.0, mutation_power), min, max);
        neuron.bias = clamp(neuron.bias + delta, min, max);
    }

    // Mutate the neuron's activation function,
    // but only if it's not an input or output neuron
    if (rng.uniform() < mutation_rate && neuron.neuron_id >= num_outputs) {
        neuron.activation = (Activation) rng.next_int(3);
    }
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
    mutation_rate = config.getDouble(
        section,
        "mutation_rate",
        0.3);
    mutation_power = config.getDouble(
        section,
        "mutation_power",
        0.8);
    replace_rate = config.getDouble(
        section,
        "replace_rate",
        0.05);
}

LinkGene LinkMutator::new_link(int input_id, int output_id) {
    // Random weight in Gaussian distribution
    double weight = clamp(new_value(mean, std), min, max);

    return {{input_id, output_id}, weight, true};
}

void LinkMutator::mutate(LinkGene &link) {
    RNG rng;
    double p = rng.uniform();
    
    // Mutate the weight value
    if (p < replace_rate) {
        link.weight = clamp(new_value(mean, std), min, max);
    } else if (p < mutation_rate + replace_rate) {
        double delta = clamp(rng.gaussian(0.0, mutation_power), min, max);
        link.weight = clamp(link.weight + delta, min, max);
    }

    // Mutate the link's enabled status
    if (rng.uniform() < mutation_rate) {
        link.is_enabled = rng.uniform() < 0.5;
    }
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