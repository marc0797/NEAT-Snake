// snakeEngine.hpp

#ifndef SNAKEENGINE_HPP
#define SNAKEENGINE_HPP

#include <algorithm>
#include <deque>
#include "rng.hpp"

// Define the Direction enum class
enum class Direction {
    Up,
    Down,
    Left,
    Right,
};

// Define the Action enum class
enum class Action {
    DoNothing,
    TurnLeft,
    TurnRight,
};

// Define the GameState enum class
enum class GameState {
    Running,
    GameOver,
    Win,
};

// Define the Coordinates struct
struct Coordinates {
    int row;
    int col;

    // Define the equality operator
    bool operator==(const Coordinates& other) const {
        return row == other.row && col == other.col;
    }
};

// Define the Snake struct
struct Snake {
    std::deque<Coordinates> body;
    short grow;

    // Returns the head of the snake
    Coordinates head() const {
        return body.front();
    }
};

// Define the SnakeEngine class
class SnakeEngine {
    public:
        // Constructor with default values
        SnakeEngine(int width = 20, int height = 20, bool allow_teleport = false)
            : width(width), height(height), allow_teleport(allow_teleport), score(0) {
            // Initialize the snake with 3 segments at
            // a random position in the middle of the board
            int row = RNG{}.next_int(height - 1);
            int col = RNG{}.next_int(width - 1);
            snake.body.push_back({row, col});
            snake.grow = 2;

            // Random direction for the snake
            current_direction = static_cast<Direction>(RNG{}.next_int(3));

            // Generate the initial food
            generate_food();
        }

        // Returns the width of the board
        int _width() const {
            return width;
        }

        // Returns the height of the board
        int _height() const {
            return height;
        }

        // Returns the snake
        const Snake& _snake() const {
            return snake;
        }

        // Returns the food
        const Coordinates& _food() const {
            return food;
        }

        // Returns the score
        int _score() const {
            return score;
        }

        // Process the action and update the game state
        GameState process(Action action) {
            // Update the direction of the snake
            current_direction = update_direction(current_direction, action);
            Coordinates new_head = get_next_head(snake, current_direction);

            // Remove the tail before checking collision
            auto tail = snake.body.back();
            snake.body.pop_back();

            // Check if the snake teleports
            if (allow_teleport) {
                if (new_head.row < 0) {
                    new_head.row = height - 1;
                } else if (new_head.row >= height) {
                    new_head.row = 0;
                } else if (new_head.col < 0) {
                    new_head.col = width - 1;
                } else if (new_head.col >= width) {
                    new_head.col = 0;
                }
            }

            if (hits_wall(new_head) || hits_snake(new_head)) {
                return GameState::GameOver;
            }

            // Check if the snake eats the food
            if (new_head == food) {
                score++;
                snake.grow++;

                // If snake body is maximum, player wins
                if (snake.body.size() + snake.grow >= width * height) {
                    return GameState::Win;
                }

                // Generate new food
                generate_food();
            }
            
            if (snake.grow) {
                // Keep the tail
                snake.body.push_back(tail);
                
                // Decrease the grow counter
                snake.grow--;
            }

            snake.body.push_front(new_head);
            return GameState::Running;
        }


    private:
        Snake snake;
        Coordinates food;
        bool allow_teleport;
        int width;
        int height;
        int score;
        Direction current_direction;

        // Update the direction of the snake
        Direction update_direction(Direction current_direction, Action action) {
            switch (action) {
                case Action::DoNothing:
                    return current_direction;
                case Action::TurnLeft:
                    switch (current_direction) {
                        case Direction::Up:
                            return Direction::Left;
                        case Direction::Left:
                            return Direction::Down;
                        case Direction::Down:
                            return Direction::Right;
                        case Direction::Right:
                            return Direction::Up;
                    }
                case Action::TurnRight:
                    switch (current_direction) {
                        case Direction::Up:
                            return Direction::Right;
                        case Direction::Right:
                            return Direction::Down;
                        case Direction::Down:
                            return Direction::Left;
                        case Direction::Left:
                            return Direction::Up;
                    }
            }
        }

        // Returns the next head of the snake
        Coordinates get_next_head(const Snake &snake, Direction direction) {
            auto head = snake.head();
            switch (direction) {
                case Direction::Up:
                    return {head.row - 1, head.col};
                case Direction::Down:
                    return {head.row + 1, head.col};
                case Direction::Left:
                    return {head.row, head.col - 1};
                case Direction::Right:
                    return {head.row, head.col + 1};
            }
        }

        void generate_food() {
            RNG rng;
            do {
                food.col = rng.next_int(width - 1);
                food.row = rng.next_int(height - 1);
            } while (hits_snake(food));
        }

        bool hits_wall(const Coordinates &c) const {
            return c.row < 0 || c.row >= height || c.col < 0 || c.col >= width;
        }

        bool hits_snake(const Coordinates &c) {
            return std::find(snake.body.begin(), snake.body.end(), c) != snake.body.end();
        }
};

#endif