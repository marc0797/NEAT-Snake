// network.cpp

#include "NEAT/network.hpp"

/**
 * Activates the network with the given inputs.
 * 
 * @param inputs The inputs to the network.
 * @return The outputs of the network.
 */
vector<double> FeedForwardNeuralNetwork::activate(const vector<double> &inputs) {
    assert(inputs.size() == input_ids.size());

    std::unordered_map<int, double> values;
    for (int i = 0; i < inputs.size(); i++) {
        int input_id = input_ids[i];
        values[input_id] = inputs[i];
    }

    // Initialize the output values to 0.0
    for (int output_id : output_ids) {
        values[output_id] = 0.0;
    }

    for (const auto &neuron : neurons) {
        double sum = neuron.bias;
        for (NeuronInput input : neuron.inputs) {
            assert(values.find(input.input_id) != values.end());
            sum += input.weight * values[input.input_id];
        }
        // Apply the activation function
        sum = ActivationFn{sum}(neuron.activation);
        values[neuron.neuron_id] = sum;
    }

    vector<double> outputs;
    for (int output_id : output_ids) {
        assert(values.find(output_id) != values.end());
        outputs.push_back(values[output_id]);
    }
    return outputs;
}

/**
 * Create a FeedForwardNeuralNetwork from a Genome
 * 
 * @param genome The genome to create the network from.
 * @return The created FeedForwardNeuralNetwork.
 */
FeedForwardNeuralNetwork create_from_genome(Genome &genome) {
    vector<int> input_ids;
    vector<int> output_ids;

    for (int i = 0; i < genome.num_inputs(); i++) {
        input_ids.push_back(-i-1);
    }

    for (int i = 0; i < genome.num_outputs(); i++) {
        output_ids.push_back(i);
    }

    vector<vector<int>> layers = 
        create_layers(input_ids, output_ids, genome.links());

    vector<Neuron> neurons;
    for (const auto &layer : layers) {
        for (int neuron_id : layer) {
            vector<NeuronInput> neuron_inputs;
            for (auto link : genome.links()) {
                LinkId link_id = link.link_id;
                if (neuron_id == link_id.output_id)
                    neuron_inputs.emplace_back(NeuronInput{link_id.input_id, link.weight});
            }
            auto neuron_opt = genome.find_neuron(neuron_id);
            assert(neuron_opt.has_value());
            NeuronGene neuron = neuron_opt.value();
            neurons.emplace_back(Neuron{neuron_id, neuron.bias, neuron.activation, neuron_inputs});
        }
    }
    return FeedForwardNeuralNetwork{std::move(input_ids), 
            std::move(output_ids), std::move(neurons)};
}


/**
 * Create the layers of the feed forward neural network.
 * 
 * @param input_ids The input ids of the network.
 * @param output_ids The output ids of the network.
 * @param links The links of the genome associated with the network.
 * @return The layers of the network.
 */
vector<vector<int>> create_layers(
    vector<int> input_ids, 
    vector<int> output_ids, 
    const vector<LinkGene> &links) {
    // Create the adjacency list and in-degree map
    std::unordered_map<int, std::unordered_set<int>> adjList;
    std::unordered_map<int, int> inDegree;

    for (const auto &link : links) {
        if (link.is_enabled) {
            LinkId link_id = link.link_id;
            adjList[link_id.input_id].insert(link_id.output_id);
            inDegree[link_id.output_id]++;
        }
    }

    // Perform a BFS to create the layers
    std::queue<int> q;
    std::unordered_map<int, int> nodeToLayer;
    vector<vector<int>> layers;

    for (int input : input_ids) {
        q.push(input);
        // The input layer is layer 0
        nodeToLayer[input] = 0;
    }

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        int currentLayer = nodeToLayer[current];

        // Add another layer if needed
        if (layers.size() <= currentLayer) {
            layers.push_back(vector<int>());
        }
        layers[currentLayer].push_back(current);

        for (int neighbor : adjList[current]) {
            // Reduce the in-degree of the neighbor
            inDegree[neighbor]--;
            // If the in-degree is 0, add it to the queue,
            // and set its layer to the current layer + 1
            if (inDegree[neighbor] == 0) {
                q.push(neighbor);
                nodeToLayer[neighbor] = currentLayer + 1;
            }
        }
    }

    // The output layer is the last layer
    for (int output : output_ids) {
        // Assume at least one output is connected to the network
        if (nodeToLayer.find(output) != nodeToLayer.end()) {
            int outputLayer = nodeToLayer[output];
            if (layers.size() <= outputLayer) {
                layers.push_back({});
            }
            // Add the output to the output layer
            if (std::find(
                layers[outputLayer].begin(), 
                layers[outputLayer].end(), output) == 
                layers[outputLayer].end()) {
                layers[outputLayer].push_back(output);
            }
        }
    }
    return std::move(layers);
}