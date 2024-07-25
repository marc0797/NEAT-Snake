// neuron_gene.cpp

#include "NEAT/genes.hpp"
#include "NEAT/rng.hpp"


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