// hsv_color_h

#ifndef HSV_COLOR_HPP
#define HSV_COLOR_HPP

#include <cmath>
#include <SFML/Graphics.hpp>

struct HsvColor {
    double h; // Hue
    double s; // Saturation
    double v; // Value

    HsvColor(double hue, double saturation, double value)
        : h(hue), s(saturation), v(value) {}

    // Convert HSV to RGB and return an sf::Color
    sf::Color to_rgb() const {
        double r = 0, g = 0, b = 0;

        int i = static_cast<int>(h / 60.0) % 6;
        double f = h / 60.0 - i;
        double p = v * (1.0 - s);
        double q = v * (1.0 - f * s);
        double t = v * (1.0 - (1.0 - f) * s);

        switch (i) {
            case 0: r = v, g = t, b = p; break;
            case 1: r = q, g = v, b = p; break;
            case 2: r = p, g = v, b = t; break;
            case 3: r = p, g = q, b = v; break;
            case 4: r = t, g = p, b = v; break;
            case 5: r = v, g = p, b = q; break;
        }

        return sf::Color(
            static_cast<sf::Uint8>(r * 255),
            static_cast<sf::Uint8>(g * 255),
            static_cast<sf::Uint8>(b * 255)
        );
    }

    // Static function to create an HsvColor from an sf::Color (RGB)
    static HsvColor from_rgb(const sf::Color& color) {
        double r = color.r / 255.0;
        double g = color.g / 255.0;
        double b = color.b / 255.0;

        double max = std::max({r, g, b});
        double min = std::min({r, g, b});
        double delta = max - min;

        double h = 0, s = 0, v = max;

        if (delta != 0) {
            s = delta / max;

            if (max == r) {
                h = 60 * (fmod(((g - b) / delta), 6));
            } else if (max == g) {
                h = 60 * (((b - r) / delta) + 2);
            } else if (max == b) {
                h = 60 * (((r - g) / delta) + 4);
            }
        }

        if (h < 0) {
            h += 360;
        }

        return HsvColor(h, s, v);
    }
};

#endif // HSV_COLOR_H
