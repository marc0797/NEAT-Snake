// activations.hpp

#ifndef ACTIVATIONS_HPP
#define ACTIVATIONS_HPP

#include <vector>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <stdexcept>

using std::vector;

enum class Activation {
    LINEAR,
    SIGMOID,
    TANH,
    RELU,
    SOFTMAX
};

class ActivationFn {
    public:
        ActivationFn(double value) : value(value) {}

        static double linear(double x) {
            return x;
        }

        static double sigmoid(double x) {
            return 1.0 / (1.0 + std::exp(-x));
        }

        static double tanh(double x) {
            return std::tanh(x);
        }

        static double relu(double x) {
            return x > 0.0 ? x : 0.0;
        }

        double operator()(Activation type) const {
            switch (type) {
                case Activation::LINEAR:
                    return linear(value);
                case Activation::SIGMOID:
                    return sigmoid(value);
                case Activation::TANH:
                    return tanh(value);
                case Activation::RELU:
                    return relu(value);
                default:
                    throw std::invalid_argument("Unknown activation function");
            }
        }


    private:
        double value;
};


#endif // ACTIVATIONS_HPP