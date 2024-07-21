// controller.hpp

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <SFML/Graphics.hpp>
#include "snakeEngine.hpp"
#include <memory>

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

std::unique_ptr<Controller> make_controller(const std::string &input) {
    if (input == "player") {
        return std::make_unique<KeyboardController>();
    }
    throw std::invalid_argument("Unknown controller type: " + input);
}

#endif // CONTROLLER_HPP