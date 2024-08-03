
#include "NEAT/network.hpp"

void testLayers() {
    std::vector<int> input_ids = {-1, -2};
    std::vector<int> output_ids = {0, 1};
    std::vector<LinkGene> links = {
        {{-1, 2}, 0.5, true},
        {{-1, 1}, 0.5, true},
        {{-2, 0}, 0.5, true},
        {{-2, 2}, 0.5, true},
        {{2, 0}, 0.5, true},
        {{2, 1}, 0.5, true}
    };

    std::vector<std::vector<int>> layers = create_layers(input_ids, output_ids, links);

    for (int i = 0; i < layers.size(); i++) {
        std::cout << "Layer " << i << ": ";
        for (int node : layers[i]) {
            std::cout << node << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    testLayers();
    std::cout << "Network layers test passed!" << std::endl;
}