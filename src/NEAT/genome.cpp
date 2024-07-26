// genome.cpp

#include "NEAT/genome.hpp"
#include "NEAT/rng.hpp"

Genome::Genome(int genome_id, Config &config) : 
    genome_id(genome_id), neuron_mutator(config), link_mutator(config) {
    _num_inputs = config.getInt(
        "DefaultGenome", 
        "num_inputs", 
        1);
    _num_outputs = config.getInt(
        "DefaultGenome", 
        "num_outputs", 
        3);
    _num_hidden = config.getInt(
        "DefaultGenome", 
        "num_hidden", 
        0);
    
    _fitness = FitnessNotCalculated;

    // Add inputs
    for (int i = 0; i < _num_inputs; i++) {
        // Inputs have negative neuron_id
        double bias = new_value(
            config.getDouble("DefaultGenome", "bias_init_mean", 0.0),
            config.getDouble("DefaultGenome", "bias_init_stddev", 1.0));
        _neurons.push_back({-i - 1, bias, Activation::SIGMOID});
    }

    // Add outputs
    for (int i = 0; i < _num_outputs; i++) {
        // Outputs have neuron_id 0 to num_outputs - 1
        NeuronGene neuron = neuron_mutator.new_neuron();
        neuron.activation = Activation::SOFTMAX;
        _neurons.push_back(neuron);
    }

    // Add hiddens if any
    for (int i = 0; i < _num_hidden; i++) {
        _neurons.push_back(neuron_mutator.new_neuron());
    }

    // Add links
    // Initially, all inputs are connected to all outputs,
    // and enabled with random weights
    for (int i = 0; i < _num_inputs; i++) {
        for (int j = 0; j < _num_outputs; j++) {
            int input_id = -i - 1;
            int output_id = j;
            _links.push_back(link_mutator.new_link(input_id, output_id));
        }
    }

    // Add links from hidden to output
    for (int i = 0; i < _num_hidden; i++) {
        for (int j = 0; j < _num_outputs; j++) {
            int input_id = _num_outputs + i;
            int output_id = j;
            _links.push_back(link_mutator.new_link(input_id, output_id));
        }
    }

    // Add links from input to hidden
    for (int i = 0; i < _num_hidden; ++i) {
        for (int j = 0; j < _num_inputs; ++j) {
            int input_id = -j - 1;
            int output_id = _num_outputs + i;
            _links.push_back(link_mutator.new_link(input_id, output_id));
        }
    }
}

/**
 * Getters
 * 
 * @return The corresponding member variable.
 */

int Genome::num_inputs() const {
    return _num_inputs;
}

int Genome::num_outputs() const {
    return _num_outputs;
}

int Genome::num_hidden() const {
    return _num_hidden;
}

float Genome::fitness() const {
    return _fitness;
}

vector<NeuronGene> Genome::neurons() {
    return _neurons;
}

const vector<NeuronGene>& Genome::neurons() const {
    return _neurons;
}

vector<LinkGene> Genome::links() {
    return _links;
}

const vector<LinkGene>& Genome::links() const {
    return _links;
}

/**
 * Add a neuron to the genome.
 * 
 * @param neuron The neuron to add.
 */
void Genome::add_neuron(const NeuronGene &neuron) {
    _neurons.push_back(neuron);
}

/**
 * Find a neuron in the genome.
 * 
 * @param neuron_id The id of the neuron to find.
 * @return The neuron if found, nullopt otherwise.
 */
optional<NeuronGene> Genome::find_neuron(int neuron_id) const {
    for (const auto &neuron : _neurons) {
        if (neuron.neuron_id == neuron_id) {
            return neuron;
        }
    }
    return std::nullopt;
}

/**
 * Add a link to the genome.
 * 
 * @param link The link to add.
 */
void Genome::add_link(const LinkGene &link) {
    _links.push_back(link);
}

/**
 * Find a link in the genome.
 * 
 * @param link_id The id of the link to find.
 * @return The link if found, nullopt otherwise.
 */
optional<LinkGene> Genome::find_link(const LinkId &link_id) const {
    for (const auto &link : _links) {
        if (link.link_id == link_id) {
            return link;
        }
    }
    return std::nullopt;
}

GenomeIndexer::GenomeIndexer() : index(0) {}

/**
 * Get the next index.
 * 
 * @return The next index.
 */
int GenomeIndexer::next() {
    return index++;
}

/**
 * Crossover two genomes.
 * 
 * @param g1 The first genome.
 * @param g2 The second genome.
 * @param config The configuration.
 * @param indexer The genome indexer.
 * @return The offspring genome.
 */
Genome crossover(const Genome &g1, const Genome &g2, Config &config, GenomeIndexer &indexer) {
    if (g2.fitness() > g1.fitness()) {
        return crossover(g2, g1, config, indexer);
    }

    Genome offspring(indexer.next(), config);

    // Inherit neuron genes
    for (const auto &n1 : g1.neurons()) {
        int neuron_id = n1.neuron_id;
        std::optional<NeuronGene> n2 = g2.find_neuron(neuron_id);
        if (!n2) {
            // Neuron is unique to g1
            offspring.add_neuron(n1);
        } else {
            // Crossover matching neurons
            offspring.add_neuron(crossover_neuron(n1, *n2));
        }
    }

    // Inherit link genes
    for (const auto &l1 : g1.links()) {
        LinkId link_id = l1.link_id;
        std::optional<LinkGene> l2 = g2.find_link(link_id);
        if (!l2) {
            // Excess or disjoint links are inherited 
            // from the fitter parent
            offspring.add_link(l1);
        } else {
            // Crossover matching links
            offspring.add_link(crossover_link(l1, *l2));
        }
    }

    return offspring;
}