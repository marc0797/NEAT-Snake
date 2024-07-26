// genome.hpp

#ifndef GENOME_HPP
#define GENOME_HPP

#include <vector>
#include <optional>
#include "NEAT/genes.hpp"
#include "NEAT/config.hpp"
#include <limits>

#define FitnessNotCalculated std::numeric_limits<float>::lowest()

using std::vector, std::optional;

class Genome {
    public:
        int genome_id;

        Genome(int genome_id, Config &config);

        // Getters
        int num_inputs() const;
        int num_outputs() const;
        int num_hidden() const;
        float fitness() const;
        vector<NeuronGene> neurons();
        const vector<NeuronGene>& neurons() const;
        vector<LinkGene> links();
        const vector<LinkGene>& links() const;

        void add_neuron(const NeuronGene &neuron);
        optional<NeuronGene> find_neuron(int neuron_id) const;
        void add_link(const LinkGene &link);
        optional<LinkGene> find_link(const LinkId &link_id) const;

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

#endif // GENOME_HPP