#pragma once
#include <algorithm>
#include <cmath>
#include <SFML/Graphics/Color.hpp>

class HSV {
    float h = 0.0f; // Hue: [0, 360)
    float s = 1.0f; // Saturation: [0, 1]
    float v = 1.0f; // Value: [0, 1]

public:
    HSV() = default;
	HSV(float h_, float s_, float v_) : h(h_), s(s_), v(v_) {}

    void setRGB(const sf::Color& rgb) {
        float r = rgb.r / 255.f;
        float g = rgb.g / 255.f;
        float b = rgb.b / 255.f;

        float max = std::max({ r, g, b });
        float min = std::min({ r, g, b });
        float delta = max - min;

        v = max;
        s = (max == 0.f) ? 0.f : delta / max;

        if (delta == 0.f) {
            h = 0.f;
        }
        else if (max == r) {
            h = 60.f * std::fmod(((g - b) / delta), 6.f);
        }
        else if (max == g) {
            h = 60.f * (((b - r) / delta) + 2.f);
        }
        else {
            h = 60.f * (((r - g) / delta) + 4.f);
        }

        if (h < 0.f) h += 360.f;
    }

    void shift(float shiftAmount = 1.0f) {
        h = std::fmod(h + shiftAmount, 360.f);
        if (h < 0.f) h += 360.f;
    }

    float getV()
    {
        return v;
    }

    void dim(float v_)
    {
        v -= v_;
    }

    static bool hasPassedHue(const HSV& original, const HSV& current, float targetHue) {
        // Normalize all to [0, 360)
        auto norm = [](float h) { return std::fmod((h < 0 ? h + 360.f : h), 360.f); };

        float h0 = norm(original.h);
        float h1 = norm(current.h);
        float target = norm(targetHue);

        // Handle wrapping properly
        if (h1 > h0) h1 -= 360.f;
        if (target > h0) target -= 360.f;

        return h1 <= target;
    }

    operator sf::Glsl::Vec3() const {
        float c = v * s;
        float x = c * (1 - std::fabs(std::fmod(h / 60.f, 2.f) - 1));
        float m = v - c;

        float r = 0, g = 0, b = 0;

        if (h < 60.f) { r = c; g = x; b = 0; }
        else if (h < 120.f) { r = x; g = c; b = 0; }
        else if (h < 180.f) { r = 0; g = c; b = x; }
        else if (h < 240.f) { r = 0; g = x; b = c; }
        else if (h < 300.f) { r = x; g = 0; b = c; }
        else { r = c; g = 0; b = x; }

        return { r + m, g + m, b + m };
    }

    operator sf::Color() const {
        float c = v * s;
        float x = c * (1 - std::fabs(std::fmod(h / 60.f, 2.f) - 1));
        float m = v - c;

        float r = 0, g = 0, b = 0;

        if (h < 60.f) { r = c; g = x; b = 0; }
        else if (h < 120.f) { r = x; g = c; b = 0; }
        else if (h < 180.f) { r = 0; g = c; b = x; }
        else if (h < 240.f) { r = 0; g = x; b = c; }
        else if (h < 300.f) { r = x; g = 0; b = c; }
        else { r = c; g = 0; b = x; }

        return sf::Color(
            static_cast<sf::Uint8>((r + m) * 255.f),
            static_cast<sf::Uint8>((g + m) * 255.f),
            static_cast<sf::Uint8>((b + m) * 255.f)
        );
    }
};
