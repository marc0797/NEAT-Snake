// genome.cpp

#include "NEAT/genome.hpp"
#include "rng.hpp"
#include <algorithm>

Genome::Genome(int genome_id, Config &config) : genome_id(genome_id), 
    neuron_mutator(config), link_mutator(config) {
    _num_inputs = config.getInt(
        "DefaultGenome", 
        "num_inputs", 
        8);
    _num_outputs = config.getInt(
        "DefaultGenome", 
        "num_outputs", 
        3);
    _num_hidden = config.getInt(
        "DefaultGenome", 
        "num_hidden", 
        0);
    _neurons = {};
    _links = {};
    _fitness = FitnessNotCalculated;
}

void Genome::config_new(Config &config) {
    // Add inputs
    for (int i = 0; i < _num_inputs; i++) {
        // Inputs have negative neuron_id, no bias, and linear activation
        _neurons.push_back({-i - 1, 0.0, Activation::LINEAR});
    }

    // Add outputs
    for (int i = 0; i < _num_outputs; i++) {
        // Outputs have neuron_id 0 to num_outputs - 1
        NeuronGene neuron = neuron_mutator.new_neuron();
        neuron.activation = Activation::LINEAR;
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

int& Genome::num_hidden() {
    return _num_hidden;
}

float& Genome::fitness() {
    return _fitness;
}

float Genome::fitness() const {
    return _fitness;
}

vector<NeuronGene>& Genome::neurons() {
    return _neurons;
}

const vector<NeuronGene>& Genome::neurons() const {
    return _neurons;
}

vector<LinkGene>& Genome::links() {
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
    neurons().push_back(neuron);
    neuron_mutator.next();
}

/**
 * Find a neuron in the genome.
 * 
 * @param neuron_id The id of the neuron to find.
 * @return The neuron if found, nullopt otherwise.
 */
optional<NeuronGene> Genome::find_neuron(int neuron_id) const {
    for (const auto &neuron : neurons()) {
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
    links().push_back(link);
}

/**
 * Find a link in the genome.
 * 
 * @param link_id The id of the link to find.
 * @return The link if found, nullopt otherwise.
 */
optional<LinkGene> Genome::find_link(const LinkId &link_id) const {
    for (const auto &link : links()) {
        if (link.link_id == link_id) {
            return link;
        }
    }
    return std::nullopt;
}

/**
 * Mutate the genome.
 * 
 * @param config The configuration.
 */
void Genome::mutate(Config &config) {
    // Get structural mutation rates from config
    double neuron_add_prob = config.getDouble(
        "DefaultGenome", 
        "neuron_add_prob", 
        0.03);
    double neuron_del_prob = config.getDouble(
        "DefaultGenome", 
        "neuron_del_prob", 
        0.01);
    double link_add_prob = config.getDouble(
        "DefaultGenome", 
        "link_add_prob", 
        0.05);
    double link_del_prob = config.getDouble(
        "DefaultGenome", 
        "link_del_prob", 
        0.01);
    
    // Get random probability
    double p = RNG{}.uniform();

    // Structural mutations
    if (p < neuron_add_prob) {
        // Add a neuron
        mutate_add_neuron();
    }

    if (p < neuron_del_prob) {
        // Remove a neuron
        mutate_remove_neuron();
    }

    if (p < link_add_prob) {
        // Add a link
        mutate_add_link();
    }

    if (p < link_del_prob) {
        // Remove a link
        mutate_remove_link();
    }

    // Mutate link genes
    for (auto &link : links()) {
        link_mutator.mutate(link);
    }

    // Mutate neuron genes
    for (auto &neuron : neurons()) {
        neuron_mutator.mutate(neuron, num_outputs());
    }
}

/**
 * Structural mutation: Add a neuron.
 * 
 */
void Genome::mutate_add_neuron() {
    if (links().empty()) {
        // No links to split
        return;
    }

    // Choose a random link to split
    RNG rng;
    LinkGene &link = rng.choose_from(links());
    // Disable the old link
    link.is_enabled = false;

    // Create a new neuron
    NeuronGene neuron = neuron_mutator.new_neuron();
    add_neuron(neuron);
    num_hidden()++;

    // Create two new links: input -> neuron with weight 1.0,
    // neuron -> output with weight of the old link, both enabled
    LinkId link_id = link.link_id;
    double weight = link.weight;
    add_link({{link_id.input_id, neuron.neuron_id}, 1.0, true});
    add_link({{neuron.neuron_id, link_id.output_id}, weight, true});
}

/**
 * Structural mutation: Remove a neuron.
 * 
 */
void Genome::mutate_remove_neuron() {
    if (num_hidden() == 0) {
        // No hidden neurons to remove
        return;
    }

    // Choose a random hidden neuron to remove
    auto &neurons = this->neurons();
    auto neuron_it = choose_random_hidden(neurons, num_outputs());

    // Remove all links connected to the neuron
    auto &links = this->links();
    links.erase(
        std::remove_if(
            links.begin(),
            links.end(),
            [neuron_it](const LinkGene &link) {
                return link.has_neuron(neuron_it);
            }),
        links.end());

    // Remove the neuron
    neurons.erase(neuron_it);
    num_hidden()--;
}

/**
 * Structural mutation: Add a link.
 * 
 */
void Genome::mutate_add_link() {
    // Get input and output links
    int input_id = choose_random_input_or_hidden(neurons(),
        num_outputs());
    int output_id = choose_random_output_or_hidden(neurons());
    LinkId link_id = {input_id, output_id};

    // Avoid duplicate links
    auto existing_link = find_link(link_id);
    if (existing_link) {
        // Enable it
        existing_link->is_enabled = true;
        return;
    }

    // Avoid cycles in the network
    if (is_cyclic(links(), input_id, output_id)) {
        return;
    }

    // Create a new link
    LinkGene new_link = link_mutator.new_link(input_id, output_id);
    add_link(new_link);
}

/**
 * Structural mutation: Remove a link.
 * 
 */
void Genome::mutate_remove_link() {
    if (links().empty()) {
        // No links to remove
        return;
    }

    // Choose a random link to remove
    RNG rng;
    auto &links = this->links();
    auto link_it = rng.choose_random(links);

    // Remove the link
    links.erase(link_it);
}

/**
 * Print the genome.
 * 
 */
void Genome::print() const {
    cout << "Genome " << genome_id << endl;
    cout << "Fitness: " << fitness() << endl;
    cout << "Neurons:";
    for (const auto &neuron : neurons()) {
        cout << "\n\t" << neuron.neuron_id << " "; 
        neuron.print();
    }
    cout << "\nLinks:";
    for (const auto &link : links()) {
        cout << "\n\t" << link.link_id.input_id << " -> " << link.link_id.output_id << " ";
        link.print();
    }
    cout << "\n\n";
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

    // Create a new genome, without neurons or links
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

/** 
 * Choose a random input or hidden neuron.
 * 
 * @param neurons The neurons to choose from.
 * @param num_outputs The number of output neurons.
 * @return The integer id of the chosen neuron.
 */
int choose_random_input_or_hidden(const vector<NeuronGene> &neurons, int num_outputs) {
    RNG rng;
    vector<int> input_or_hidden;
    for (const auto &neuron : neurons) {
        if (neuron.neuron_id < 0 || neuron.neuron_id >= num_outputs) {
            input_or_hidden.push_back(neuron.neuron_id);
        }
    }
    return rng.choose_from(input_or_hidden);
}

/** 
 * Choose a random output or hidden neuron.
 * 
 * @param neurons The neurons to choose from.
 * @return The integer id of the chosen neuron.
 */
int choose_random_output_or_hidden(const vector<NeuronGene> &neurons) {
    RNG rng;
    vector<int> output_or_hidden;
    for (const auto &neuron : neurons) {
        if (neuron.neuron_id >= 0) {
            output_or_hidden.push_back(neuron.neuron_id);
        }
    }
    return rng.choose_from(output_or_hidden);
}

/** 
 * Choose a random hidden neuron.
 * 
 * @param neurons The neurons to choose from.
 * @param num_outputs The number of output neurons.
 * @return The iterator to the chosen neuron.
 */
vector<NeuronGene>::iterator choose_random_hidden(vector<NeuronGene> &neurons, int num_outputs) {
    RNG rng;
    vector<NeuronGene>::iterator it;
    do {
        it = rng.choose_random(neurons);
    } while (it->neuron_id < num_outputs);
    return it;
}

/** 
 * Check if adding a link would create a cycle.
 * 
 * @param links The links in the genome.
 * @param input_id The id of the input neuron.
 * @param output_id The id of the output neuron.
 * @return True if a cycle would be created, false otherwise.
 */
bool is_cyclic(const vector<LinkGene> &links, int input_id, int output_id) {
    if (input_id == output_id) {
        return true;
    }

    // Check if there is a path from output_id to input_id by recursion
    for (const auto &link : links) {
        if (link.link_id.input_id == output_id) {
            if (is_cyclic(links, link.link_id.output_id, input_id)) {
                return true;
            }
        }
    }
    return false;
}