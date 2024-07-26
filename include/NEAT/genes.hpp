// neuron_gene.hpp

#ifndef NEAT_NEURON_GENE_HPP
#define NEAT_NEURON_GENE_HPP

#include <vector>
#include "NEAT/config.hpp"

using std::vector;

enum class Activation {
    SIGMOID,
    TANH,
    RELU,
    SOFTMAX
};

// Neuron gene, struct is easier to work with than class
struct NeuronGene {
    int neuron_id;
    double bias;
    Activation activation;
};

// LinkId gene
struct LinkId {
    int input_id;
    int output_id;

    bool operator==(const LinkId &rhs) const {
        return input_id == rhs.input_id && output_id == rhs.output_id;
    }
    
};

// Link gene
struct LinkGene {
    LinkId link_id;
    double weight;
    bool is_enabled;

    bool has_neuron(vector<NeuronGene>::iterator it) {
        return link_id.input_id == it->neuron_id || link_id.output_id == it->neuron_id;
    }
};

// Neuron indexer and mutator
class NeuronMutator {
    public:
        NeuronMutator(Config &config);
        NeuronGene new_neuron();
    private:
        // For generating new neurons
        int index;
        Activation activation;
        double mean;
        double std;
        double min;
        double max;
};

// Link indexer and mutator
class LinkMutator {
    public:
        LinkMutator(Config &config);
        LinkGene new_link(int input_id, int output_id);
    private:
        // For generating new links
        int index;
        double mean;
        double std;
        double min;
        double max;
};

double new_value(double mean, double std);
double clamp(double value, double min, double max);

NeuronGene crossover_neuron(const NeuronGene &n1, const NeuronGene &n2);
LinkGene crossover_link(const LinkGene &l1, const LinkGene &l2);

#endif // NEAT_NEURON_GENE_HPP