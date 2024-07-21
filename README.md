# NEAT-Snake

This repository contains an implementation of the NeuroEvolution of Augmenting Topologies (NEAT) genetic algorithm to play the classic Snake game. The project is developed in C++ and demonstrates the application of evolutionary algorithms to solve game-playing problems.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)

## Introduction

In short, the NEAT algorithm is a method for evolving neural networks using genetic algorithms. The advantage over other GA is that, even though it starts with a simple neural network structure, it can gradually increase its complexity by adding new nodes and connections.
This project uses NEAT to train a neural network to play the Snake game, where the goal is to control the snake to collect food while avoiding collisions with the walls and itself.

## Features
- **NEAT algorithm**: A simple implementation of NEAT to evolve neural networks.
- **Snake Game**: The Snake game in C++.
- **Visualization**: Real-time visualization of the Snake game and the performance of the neural network.
- **Configurable Parameters**: Easily adjustable for the NEAT algorithm and/or the game settings.

## Installation

### Pre-requisites:

Ensure you have `cmake` and a C++ compiler installed on your system.

1. Clone the Repository:
```bash
  git clone https://github.com/marc0797/NEAT-Snake.git
  cd NEAT-Snake
```
2. Build the project: Use `CMake` to configure and build the project.
```bash
mkdir build
cd build
cmake ..
make
```
3. Run the Executable:
```bash
./NEAT-Snake <player|ai> [-options]
```

## Usage

When running the executable, indicate which game mode would you like to execute: `player` or `ai`, as an additional argument.

If `player` is specified, a Snake game window will appear, and the game will start as soon as it detects a keyboard input.
To play, use the keyboard arrows to turn left or right.

**Additional options:**
- Width: Set the width of the grid, with `-w <width>`.
- Height: Set the height of the grid, with `-h <height>`.
- FPS: Set the framerate of the game, with `-f <FPS>`.
- Teleport: Allow the snake to teleport, with `-t`.
- Window: Set the window size (for the moment, it is a square), with `-s <size>`.
- Score: Display the score on the screen always, with `-z`.

When the game is over, the screen is frozen unil the window is closed, or the player presses a key.
If ***r*** is pressed, the game restarts with the same options.

If `ai` is specified, the same Snake game window appears, but the controller is now the AI. (TO-DO).