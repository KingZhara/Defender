#include "../DisplayManager.h"
#include <iostream>
#include <SFML/OpenGL.hpp>

#include "../../State/StageState.h"


sf::Vector2u DisplayManager::resolution = getMaxAspectResolution(
    sf::VideoMode::getDesktopMode().width,
    sf::VideoMode::getDesktopMode().height - 200 /*space for taskbar, wont work for anything besides small horizontal taskbar*/,
    static_cast<int>(COMN::aspectRatio.x),
    static_cast<int>(COMN::aspectRatio.y)
);
// The game window
sf::RenderWindow* DisplayManager::window = nullptr;
sf::RenderTexture* DisplayManager::frameA = nullptr;
sf::RenderTexture* DisplayManager::frameB = nullptr;
sf::Texture* DisplayManager::textures = nullptr; // In-game textures
sf::Shader* DisplayManager::smoothShader = nullptr;
sf::View DisplayManager::viewport = sf::View(sf::FloatRect(0, 0, COMN::resolution.x, COMN::resolution.y));
bool DisplayManager::outputTexture = true;



void DisplayManager::initialize()
{
    // Window resolution chosen based on screen resolution and aspect ratio
    // Set view to emulate virtual resolution (same world units everywhere)
    viewport.setSize(COMN::resolution.x, COMN::resolution.y);
    viewport.setCenter(COMN::resolution.x / 2.f, COMN::resolution.y / 2.f);

	//std::cout << "VP: " << viewport.getSize().x << ' ' << viewport.getSize().y << '\n';
    // Create the window at scaled resolution
    window = new sf::RenderWindow(sf::VideoMode(resolution.x, resolution.y),
        "Defender", sf::Style::Titlebar | sf::Style::Close);
    frameA = new sf::RenderTexture;
    frameB = new sf::RenderTexture;
    smoothShader = new sf::Shader;

    window->setView(viewport);
    frameA->setView(viewport);
    frameB->setView(viewport);

    // This is your brain on drugs pookie
    smoothShader->loadFromFile("./res/shaders/epilepsySmooth.frag",
        sf::Shader::Type::Fragment);

    if (!frameA->create(resolution.x, resolution.y) ||
        !frameB->create(resolution.x, resolution.y))
        std::cerr << "Failed to create render textures.\n";

    textures = textures = loadSpritesheet();

    frameA->setSmooth(false);
    frameB->setSmooth(false);
    textures->setSmooth(false);

    frameB->clear(sf::Color::Black);
    frameB->display();

    window->setFramerateLimit(60);
}

void DisplayManager::clean()
{
    delete textures;
    delete window;
	delete frameA;
	delete frameB;
	delete smoothShader;
}

void DisplayManager::draw(sf::Drawable& all)
{
    // Draw the current frame
    (outputTexture ? frameA : frameB)->clear();
	(outputTexture ? frameA : frameB)->draw(all);

    // I want to note that a compute shader calculating mean entropy between frames would be ideal, but this is the best I can do for now.
    smoothShader->setUniform("frameA", (outputTexture ? frameA : frameB)->getTexture());
    smoothShader->setUniform("lastFrame", (outputTexture ? frameB : frameA)->getTexture());
    smoothShader->setUniform("maxDelta", 1.f); // 0.3 is a safe-ish value

	// Apply the shader after drawing the current frame
	//(outputTexture ? frameA : frameB)->draw(sf::Sprite((outputTexture ? frameA : frameB)->getTexture()), smoothShader);

    //std::cout << "VP: " << viewport.getSize().x << ' ' << viewport.getSize().y << '\n';
    //window->setView(viewport);
    frameA->setView(viewport);
    frameB->setView(viewport);
    // Assumes getRenderTarget() has been called, and the current frame has been drawn to
    (outputTexture ? frameA : frameB)->display();

    window->clear();
    window->draw(sf::Sprite((outputTexture ? frameA : frameB)->getTexture()), smoothShader);
    window->display();

	outputTexture = !outputTexture;
}

sf::Texture * DisplayManager::loadSpritesheet()
{
    auto tex = new sf::Texture;
    tex->loadFromFile("res/Spritesheet.png");

    Animation::setTexture(tex);

    return tex;
}

sf::Vector2u DisplayManager::getMaxAspectResolution(int screenWidth,
                                                    int screenHeight,
                                                    int aspectWidth,
                                                    int aspectHeight)
{
    sf::Vector2u resolution;

    // Compute width-first scaling
    int heightForMaxWidth = (screenWidth * aspectHeight) / aspectWidth;

    if (heightForMaxWidth <= screenHeight)
    {
        // The width-first scaling fits
        resolution.x = screenWidth;
        resolution.y = heightForMaxWidth;
    }
    else
    {
        // The height-first scaling is needed
        resolution.x = (screenHeight * aspectWidth) / aspectHeight;
        resolution.y = screenHeight;
    }

    return resolution;
}
