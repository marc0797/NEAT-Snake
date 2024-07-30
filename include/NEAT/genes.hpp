// genes.hpp

#ifndef NEAT_GENES_HPP
#define NEAT_GENES_HPP

#include <vector>
#include "NEAT/config.hpp"

using std::vector;

enum class Activation {
    LINEAR,
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

    void print() const {
        string act;
        switch (activation) {
            case Activation::LINEAR:
                act = "LINEAR";
                break;
            case Activation::SIGMOID:
                act = "SIGMOID";
                break;
            case Activation::TANH:
                act = "TANH";
                break;
            case Activation::RELU:
                act = "RELU";
                break;
            case Activation::SOFTMAX:
                act = "SOFTMAX";
                break;
        }
        std::cout << "Bias: " << bias << " Activation: " << act;
    }
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

    bool has_neuron(vector<NeuronGene>::iterator it) const {
        return link_id.input_id == it->neuron_id || link_id.output_id == it->neuron_id;
    }

    void print() const {
        std::cout << "Weight: " << weight << " Enabled: ";
        std::cout << std::boolalpha << is_enabled;
    }
};

// Neuron indexer and mutator
class NeuronMutator {
    public:
        NeuronMutator(Config &config);
        NeuronGene new_neuron();
        int next();
        void mutate(NeuronGene &neuron, int num_outputs);
    private:
        // For generating new neurons
        int index;
        Activation activation;
        double mean;
        double std;
        double min;
        double max;

        // For mutating neurons
        double mutation_rate;
        double mutation_power;
        double replace_rate;
};

// Link indexer and mutator
class LinkMutator {
    public:
        LinkMutator(Config &config);
        LinkGene new_link(int input_id, int output_id);
        void mutate(LinkGene &link);
    private:
        // For generating new links
        int index;
        double mean;
        double std;
        double min;
        double max;

        // For mutating links
        double mutation_rate;
        double mutation_power;
        double replace_rate;
};

double new_value(double mean, double std);
double clamp(double value, double min, double max);

NeuronGene crossover_neuron(const NeuronGene &n1, const NeuronGene &n2);
LinkGene crossover_link(const LinkGene &l1, const LinkGene &l2);

#endif // NEAT_GENES_HPP