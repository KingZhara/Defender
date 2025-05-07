#pragma once
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>

#include "VisualComponent/Animation.h"
#include "common.h"

namespace sf
{
    class Texture;
    class View;
}

class DisplayManager
{
public:
    template <typename T>
    struct Color
    {
        T r, g, b;

        constexpr Color(T r_, T g_, T b_) : r(r_), g(g_), b(b_) {}

        operator sf::Color() const
        {
            return sf::Color((uint8_t)r, (uint8_t)g, (uint8_t)b);
        }

        operator sf::Glsl::Vec3() const
        {
            return sf::Glsl::Vec3(r / 255.0f, g / 255.0f, b / 255.0f);
        }
    };

    static sf::View &getView() { return viewport; }

    static void resetViewPos()
    {
        viewport.setCenter(viewport.getSize().x / 2, viewport.getSize().y / 2);
    }

    static sf::RenderWindow * getWindow() { return window; }
    static void               initialize();
    static void               clean();
    static void               draw(sf::Drawable &all);

    static sf::Texture*       getTexture() { return textures; }

private:
    static sf::Vector2u       resolution; // Window resolution
    static sf::RenderWindow * window;
    static sf::Texture *      textures; // In-game textures
    static sf::RenderTexture *frameA;
    static sf::RenderTexture *frameB;
    static sf::Shader *       smoothShader;
    static sf::View           viewport;
    static bool               outputTexture;

    static sf::Texture *loadSpritesheet();
    static sf::Vector2u getMaxAspectResolution(int screenWidth,
                                               int screenHeight,
                                               int aspectWidth,
                                               int aspectHeight);
};
