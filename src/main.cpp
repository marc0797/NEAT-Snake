#include <SFML/Graphics.hpp>
#include "snakeEngine.hpp"
#include "hsv_color.hpp"


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
            config.snake_color = { 0, 112, 246 };
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

        void draw_snake() {
            const auto &snake = engine._snake();
            for (int i = 0; i < snake.body.size(); i++) {
                auto segment = snake.body[i];
                draw_field(segment, config.snake_color);
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

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Snake Game");
    SnakeEngine engine{20,20};
    GameRenderer renderer{window, engine, GameRendererConfig{}};

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else {
                // Process events
            }
        }
        renderer.draw();
        window.display();
    }
    return 0;
}
