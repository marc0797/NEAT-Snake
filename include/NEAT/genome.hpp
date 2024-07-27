// genome.hpp

#ifndef GENOME_HPP
#define GENOME_HPP

#include <vector>
#include <optional>
#include "NEAT/genes.hpp"
#include "NEAT/config.hpp"
#include <limits>

#define FitnessNotCalculated std::numeric_limits<float>::lowest()

using std::vector, std::optional, std::cout, std::endl;

class Genome {
    public:
        int genome_id;

        Genome(int genome_id, Config &config);

        // Getters
        int num_inputs() const;
        int num_outputs() const;
        int &num_hidden();
        float fitness() const;
        vector<NeuronGene> neurons();
        const vector<NeuronGene>& neurons() const;
        vector<LinkGene> links();
        const vector<LinkGene>& links() const;

        void add_neuron(const NeuronGene &neuron);
        optional<NeuronGene> find_neuron(int neuron_id) const;
        void add_link(const LinkGene &link);
        optional<LinkGene> find_link(const LinkId &link_id) const;

        void mutate(Config &config);
        void mutate_add_neuron();
        void mutate_remove_neuron();
        void mutate_add_link();
        void mutate_remove_link();

        void print() const;
        

    private:
        int _num_inputs;
        int _num_outputs;
        int _num_hidden;
        NeuronMutator neuron_mutator;
        LinkMutator link_mutator;
        vector<NeuronGene> _neurons;
        vector<LinkGene> _links;
        float _fitness;
};

class GenomeIndexer {
    public:
        GenomeIndexer();

        int next();

    private:
        int index;
};

Genome crossover(const Genome &g1, const Genome &g2, Config &config, GenomeIndexer &indexer);

int choose_random_input_or_hidden(const vector<NeuronGene> &neurons, int num_outputs);
int choose_random_output_or_hidden(const vector<NeuronGene> &neurons);
vector<NeuronGene>::iterator choose_random_hidden(const vector<NeuronGene> &neurons, int num_outputs);

bool is_cyclic(const vector<LinkGene> &links, int input_id, int output_id);

#endif // GENOME_HPP