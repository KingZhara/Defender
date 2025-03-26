#include "../DisplayManager.h"
#include <iostream>

#include "../../State/StageState.h"


sf::Vector2u DisplayManager::resolution = getMaxAspectResolution(
    sf::VideoMode::getDesktopMode().width,
    sf::VideoMode::getDesktopMode().height, COMN::aspectRatio.x, COMN::aspectRatio.y);
// The game window
sf::RenderWindow* DisplayManager::window = nullptr;
sf::RenderTexture* DisplayManager::currentFrame;
sf::RenderTexture* DisplayManager::previousFrame;
sf::RenderTexture* DisplayManager::output;
sf::Texture* DisplayManager::textures = nullptr; // In-game textures
sf::Shader* DisplayManager::smoothShader;
sf::View DisplayManager::viewport = sf::View(sf::FloatRect(0, 0, COMN::resolution.x, COMN::resolution.y));



void DisplayManager::initialize()
{
	window = new sf::RenderWindow(sf::VideoMode(resolution.x, resolution.y),
        "Defender", sf::Style::Titlebar | sf::Style::Close);
    currentFrame = new sf::RenderTexture;
    previousFrame = new sf::RenderTexture;
    output = new sf::RenderTexture;
    smoothShader = new sf::Shader;


    // This is your brain on drugs pookie
    smoothShader->loadFromFile("./res/shaders/epilepsySmooth.frag",
        sf::Shader::Type::Fragment);



    if (!currentFrame->create(resolution.x, resolution.y) ||
        !previousFrame->create(resolution.x, resolution.y) ||
        !output->create(resolution.x, resolution.y))
        std::cerr << "Failed to create render textures.\n";

    textures = textures = loadSpritesheet();

    currentFrame->setSmooth(false);
    previousFrame->setSmooth(false);
    output->setSmooth(false);
    textures->setSmooth(false);

    previousFrame->clear(sf::Color::Black);
    previousFrame->display();


    currentFrame->setView(viewport);

    window->setFramerateLimit(60);
}

void DisplayManager::clean()
{
    delete textures;
}

void DisplayManager::draw()
{
    // Assumes getRenderTarget() has been called, and the current frame has been drawn to
    currentFrame->display();

    smoothShader->setUniform("currentFrame", currentFrame->getTexture());
    smoothShader->setUniform("lastFrame", previousFrame->getTexture());
    smoothShader->setUniform("maxDelta", 1.0f); // 0.3 is a safe-ish value

    output->clear();
    output->draw(sf::Sprite(currentFrame->getTexture()), smoothShader);
    output->display();

    window->clear();
    window->draw(sf::Sprite(output->getTexture()));
    window->display();

    previousFrame->clear();
    previousFrame->draw(sf::Sprite(output->getTexture()));
    previousFrame->display();

    currentFrame->clear();
}

sf::RenderTexture * DisplayManager::getRenderTarget()
{
    return currentFrame;
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
