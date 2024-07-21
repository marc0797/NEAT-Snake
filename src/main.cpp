#include <SFML/Graphics.hpp>
#include "snakeEngine.hpp"
#include "hsv_color.hpp"
#include "controller.hpp"
#include "ticker.hpp"
#include <cassert>

// Game configuration
static int WIDTH = 30;
static int HEIGHT = 30;
static double FPS = 10.0;
static bool ALLOW_TELEPORT = false;
static int window_size = 800;


// Define the GameRendererConfig class
class GameRendererConfig {
    public:
        GameRendererConfig() {}

        sf::Vector2f grid_size;
        sf::Vector2f grid_offset;
        sf::Vector2f field_size;
        sf::Vector2f field_offset;
        sf::Color background_color;
        sf::Color stroke_color;
        sf::Color snake_color;
        sf::Color food_color;
};

// Define the GameRenderer class
class GameRenderer {
    public:
        GameRenderer(sf::RenderWindow &window, const SnakeEngine &engine,
        GameRendererConfig config) : window(window), engine(engine), config() {
            set_config();
        }

        // Set config values from window size and engine dimensions
        void set_config() {
            // Grid size is 80% of the window size, with a 10% offset 
            // on each side
            config.grid_size = {window.getSize().x * 0.8f, 
                            window.getSize().y * 0.8f};
            config.grid_offset = {window.getSize().x * 0.1f, 
                            window.getSize().y * 0.1f};
            // Field size is the grid size divided by the number of 
            // rows and columns, minus the stroke width
            config.field_size = {
                config.grid_size.x / engine._width() - 2,
                config.grid_size.y / engine._height() - 2
            };
            config.field_offset = {1, 1};
            config.background_color = { 23, 23, 33 };
            config.stroke_color = { 57, 57, 67 };
            config.snake_color = { 228, 63, 63 };
            config.food_color = { 72, 201, 176 };
        }

        void draw() {
            window.clear(config.background_color);
            draw_grid();
            draw_snake();
            draw_food();
        }

        void draw_grid() {
            sf::RectangleShape stroke{};
            stroke.setSize(config.grid_size);
            stroke.setPosition(config.grid_offset);
            stroke.setFillColor(config.stroke_color);
            window.draw(stroke);

            for (int i = 0; i < engine._height(); i++) {
                for (int j = 0; j < engine._width(); j++) {
                    draw_field({i, j}, config.background_color);
                }
            }
        }

        sf::Color luminance_wheel(sf::Color base, double position,
                            double min = 0.5, double max = 0.75) {
            auto hsv = HsvColor::from_rgb(base);
            hsv.v = min + (max - min) * position;
            assert(hsv.v >= 0.0 && hsv.v <= 1.0);
            return hsv.to_rgb();
        }

        sf::Color color_wheel(sf::Color base, double position) {
            auto hsv = HsvColor::from_rgb(base);
            hsv.h = fmod(hsv.h + 360 * position, 360);
            return hsv.to_rgb();
        }

        void draw_snake() {
            const auto &snake = engine._snake();
            for (int i = 0; i < snake.body.size(); i++) {
                auto segment = snake.body[i];
                double position = 1.0 - (double)i / (double)snake.body.size();
                draw_field(segment, color_wheel(config.snake_color, position));
            }
        }

        void draw_food() {
            draw_field(engine._food(), config.food_color);
        }

        void draw_field(Coordinates c, sf::Color color) {
            sf::RectangleShape shape{};
            shape.setSize(config.field_size);
            shape.setPosition(to_position(c));
            shape.setFillColor(color);
            window.draw(shape);
        }
            

    private:
        sf::RenderWindow &window;
        const SnakeEngine &engine;
        GameRendererConfig config;

        sf::Vector2f to_position(Coordinates c) {
            // Calculate the position of the field, such that
            // the offset of the field is what makes the stroke
            return {
                config.grid_offset.x + c.col * (config.field_size.x + 2)
                + config.field_offset.x,
                config.grid_offset.y + c.row * (config.field_size.y + 2)
                + config.field_offset.y
            };
        }
};

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: ./NEAT_Snake <player|ai>\n");
        return -1;
    }

    sf::RenderWindow window(
        sf::VideoMode(window_size, window_size), 
        "Snake Game");

    auto controller = make_controller(argv[1]);
    Ticker ticker{FPS};
    SnakeEngine engine{WIDTH, HEIGHT};
    GameRenderer renderer{window, engine, GameRendererConfig{}};
    GameState state = GameState::Running;

    while (window.isOpen()) {
        sf::Event event{};

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else {
                controller->on_key_pressed(event);
            }
        }

        if (ticker.tick()) {
            state = engine.process(controller->get_action());
        }

        if (state != GameState::Running) {
            // If state is win, print a message on the window, and freeze the screen
            sf::Font font;
            font.loadFromFile("assets/Arial.ttf");
            sf::Text text;
            text.setFont(font);
            if (state == GameState::GameOver) {
                text.setString("Game Over!\nFinal Score: " 
                    + std::to_string(engine._score()));
            } else {
                text.setString("You Win!\nFinal Score: " 
                    + std::to_string(engine._score()));
            }
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::White);
            text.setPosition(window.getSize().x * 0.5f, 
                window.getSize().y * 0.5f);
            window.draw(text);
            window.display();
            // Wait for the window to be closed, or for the user to press a key.
            // If 'r' is pressed, restart the game
            sf::Event event{};
            while (window.waitEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    return 0;
                } else if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::R) {
                        engine = SnakeEngine{WIDTH, HEIGHT};
                        state = GameState::Running;
                        break;
                    } else {
                        window.close();
                        return 0;
                    }
                }
            }
        }

        renderer.draw();
        window.display();
    }
    return 0;
}
