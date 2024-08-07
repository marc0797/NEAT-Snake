// genes.hpp

#ifndef NEAT_GENES_HPP
#define NEAT_GENES_HPP

#include "NEAT/activations.hpp"
#include "NEAT/config.hpp"

using std::vector;

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

    friend std::ostream& operator<<(std::ostream &os, const NeuronGene &n) {
        // Print neuron id
        os << n.neuron_id << "\n";

        // Print bias and activation
        os << n.bias << "\n";
        os << (int) n.activation << "\n";

        return os;
    }

    friend std::istream& operator>>(std::istream &is, NeuronGene &n) {
        // Read neuron id
        is >> n.neuron_id;

        // Read bias and activation
        is >> n.bias;
        int activation;
        is >> activation;
        n.activation = (Activation) activation;

        return is;
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

    friend std::ostream& operator<<(std::ostream &os, const LinkGene &l) {
        // Print link id
        os << l.link_id.input_id << "\n";
        os << l.link_id.output_id << "\n";

        // Print weight and enabled status
        os << l.weight << "\n";
        os << l.is_enabled << "\n";

        return os;
    }

    friend std::istream& operator>>(std::istream &is, LinkGene &l) {
        // Read link id
        is >> l.link_id.input_id;
        is >> l.link_id.output_id;

        // Read weight and enabled status
        is >> l.weight;
        is >> l.is_enabled;

        return is;
    }
};

// Neuron indexer and mutator
class NeuronMutator {
    public:
        NeuronMutator(Config &config);
        
        // Default constructor for NeuronMutator
        NeuronMutator() : index(0), activation(Activation::SIGMOID), 
            mean(0.0), std(1.0), min(-30.0), max(30.0),
            mutation_rate(0.3), mutation_power(0.8), 
            replace_rate(0.05) {}

        NeuronGene new_neuron();
        int next();
        void mutate(NeuronGene &neuron, int num_outputs);

        friend std::ostream& operator<<(std::ostream &os, const NeuronMutator &n);
        friend std::istream& operator>>(std::istream &is, NeuronMutator &n);
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

        // Default constructor for LinkMutator
        LinkMutator() : index(0), mean(0.0), std(1.0), min(-30.0), max(30.0),
            mutation_rate(0.3), mutation_power(0.8), replace_rate(0.05) {}

        LinkGene new_link(int input_id, int output_id);
        void mutate(LinkGene &link);

        friend std::ostream& operator<<(std::ostream &os, const LinkMutator &l);
        friend std::istream& operator>>(std::istream &is, LinkMutator &l);
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