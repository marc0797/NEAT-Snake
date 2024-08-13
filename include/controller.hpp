// controller.hpp

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <SFML/Graphics.hpp>
#include "snakeEngine.hpp"
#include "NEAT/population.hpp"
#include "NEAT/network.hpp"
#include <memory>

#define sgn(x) ((x > 0) - (x < 0))

class Controller {
    public:
        virtual ~Controller() = default;
        virtual void on_key_pressed(sf::Event &) {}
        virtual Action get_action() = 0;
};

class KeyboardController : public Controller {
    public:
        void on_key_pressed(sf::Event &event) override {
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Left:
                        next_action = Action::TurnLeft;
                        break;
                    case sf::Keyboard::Right:
                        next_action = Action::TurnRight;
                        break;
                    default:
                        break;
                }
            }
        }

        Action get_action() override {
            auto action = next_action;
            next_action = Action::DoNothing;
            return action;
        }

    private:
        Action next_action;
};

class AIController : public Controller {
    public:
        AIController(SnakeEngine &engine, FeedForwardNeuralNetwork nn) :
            engine(engine), nn(nn), growFPS(0) {}

        void on_key_pressed(sf::Event &event) override {
            if (event.type == sf::Event::KeyPressed) {
                switch(event.key.code) {
                    case sf::Keyboard::Up:
                        growFPS += 5;
                        break;
                    case sf::Keyboard::Down:
                        growFPS -= 5;
                        break;
                    default:
                        break;
                }
            }
        }
        
        Action get_action() override {
            auto inputs = extract_inputs(engine);
            auto outputs = nn.activate(inputs);

            // Find maximum
            int max_index = 0;
            for (int i = 1; i < outputs.size(); i++) {
                if (outputs[i] > outputs[max_index]) {
                    max_index = i;
                }
            }
            // cout << "Inputs: [ ";
            // for (auto input : inputs) {
            //     cout << input << " ";
            // }
            // cout << "]";
            // cout << "\tOutputs: [ ";
            // for (auto output : outputs) {
            //     cout << output << " ";
            // }
            // cout << "]" << endl;
            return static_cast<Action>(max_index);
        }

        vector<double> extract_inputs(const SnakeEngine &engine) {
            vector<double> inputs;
            Coordinates head = engine._snake().head();
            Coordinates food = engine._food();

            Coordinates dist{sgn(food.row - head.row), sgn(food.col - head.col)};

            vector<Coordinates> offsets = {
                {0, -1}, {1, 0}, {0, 1}, {-1, 0}
            };

            // Add 1 if it sees food, 0 otherwise
            for (int i = 0; i < 4; i++) {
                int rel_direction = (engine._direction() + i) % 4;
                if (dist == offsets[rel_direction]) {
                    inputs.push_back(1.0);
                }
                else {
                    inputs.push_back(0.0);
                }
            }

            // // Normalized distance to the food
            // inputs.push_back((food.row - head.row) / engine._height());
            // inputs.push_back((food.col - head.col) / engine._width());

            // // Snake's current direction
            // for (int i = 0; i < 4; i++) {
            //     if (i == engine._direction()) {
            //         inputs.push_back(1.0);
            //     }
            //     else {
            //         inputs.push_back(0.0);
            //     }
            // }

            // Add the (normalized) distances to the wall or snake body
            for (int i = 0; i < 4; i++) {
                int rel_direction = (engine._direction() + i) % 4;
                float distance = 0.0;
                Coordinates current = head;
                current.row += offsets[rel_direction].row;
                current.col += offsets[rel_direction].col;
                // do {
                //     distance++;
                //     current.row += offsets[rel_direction].row;
                //     current.col += offsets[rel_direction].col;
                // } while(!engine.hits_wall(current) && !engine.hits_snake(current));
                // inputs.push_back(std::exp(-distance / engine._width()));
                if (engine.hits_wall(current) || engine.hits_snake(current)) {
                    inputs.push_back(1.0);
                }
                else {
                    inputs.push_back(0.0);
                }
            }

            return inputs;
        }
    private:
        SnakeEngine &engine;
        FeedForwardNeuralNetwork nn;
        int growFPS;
};

std::unique_ptr<Controller> make_controller(const std::string &input, SnakeEngine &engine) {
    if (input == "player") {
        return std::make_unique<KeyboardController>();
    } else if (input == "ai") {
        Population population("population.txt");
        Genome best_genome = population.best_genome();
        FeedForwardNeuralNetwork nn = create_from_genome(best_genome);
        return std::make_unique<AIController>(engine, std::move(nn));
    }
    else throw std::invalid_argument("Unknown controller type: " + input);
}

void update_fitness(Genome &genome) {
    RNG rng;
    int max_steps = 1000;

    SnakeEngine engine(rng);
    FeedForwardNeuralNetwork nn =
        create_from_genome(genome);
    AIController controller(engine, std::move(nn));

    GameState game_state = GameState::Running;
    for (int step = 0; step < max_steps && 
            game_state == GameState::Running; step++) {
        Action action = controller.get_action();
        game_state = engine.process(action);
    }
    genome.fitness() = engine._score();
}

#endif // CONTROLLER_HPP