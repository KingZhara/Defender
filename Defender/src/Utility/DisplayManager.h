#pragma once
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>

#include "Animation.h"
#include "common.h"

namespace sf
{
    class Texture;
    class View;
}

class DisplayManager
{
public:
    static sf::View &getView() { return viewport; }

    static void resetViewPos()
    {
        viewport.setCenter(viewport.getSize().x / 2, viewport.getSize().y / 2);
    }

    static sf::RenderWindow *getWindow() { return window; }
    static void              initialize();
    static void              clean();
    static void              draw(sf::Drawable &all);

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
