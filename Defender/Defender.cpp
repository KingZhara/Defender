#include <SFML/Graphics.hpp>

#include "Defender.hpp"

#include "src/Utility/Action.h"

void         setAction(Action &actions, sf::Keyboard::Key key, bool pressed);
sf::Texture *loadSpritesheet();
sf::Vector2u getMaxAspectResolution(int screenWidth,
                                    int screenHeight,
                                    int aspectWidth,
                                    int aspectHeight);

int main()
{
    // 73 : 60
    sf::Vector2u resolution = getMaxAspectResolution(
            sf::VideoMode::getDesktopMode().width,
            sf::VideoMode::getDesktopMode().height, 73, 60);
    // The game window
    sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y),
                            "Defender", sf::Style::Titlebar | sf::Style::Close);
    // The game itself
    Game game;
    // Player actions; passed throughout the tick pipeline as special handling is included in AttractState
    Action actions;
    // The games viewport
    auto viewport = sf::View(sf::FloatRect(0, 0, 292, 240));
    // The Clock used for deltatime
    sf::Clock clock;
    // The games sprite textures
    sf::Texture *textures = loadSpritesheet();

    sf::RenderTexture currentFrame;
    sf::RenderTexture previousFrame;
    sf::RenderTexture output;

    sf::Shader smoothShader;

    // This is your brain on drugs pookie
    smoothShader.loadFromFile("./res/shaders/epilepsySmooth.frag",
                              sf::Shader::Type::Fragment);

    if (!currentFrame.create(resolution.x, resolution.y) ||
        !previousFrame.create(resolution.x, resolution.y) ||
        !output.create(resolution.x, resolution.y))
        std::cerr << "Failed to create render textures.\n";

    currentFrame.setSmooth(false);
    previousFrame.setSmooth(false);
    output.setSmooth(false);
    textures->setSmooth(false);

    previousFrame.clear(sf::Color::Black);
    previousFrame.display();

    // Set the view for correct scaling with window size
    StageState::setView(window, viewport);

    UserInterface::initialize();

    window.setFramerateLimit(60);

    // Min game loop
    while (window.isOpen())
    {
        // The windows events
        sf::Event e;
        while (window.pollEvent(e))
        {
            switch (e.type)
            {
            case sf::Event::Closed:
                break;

            case sf::Event::KeyPressed:
                setAction(actions, e.key.code, true);
                break;

            case sf::Event::KeyReleased:
                setAction(actions, e.key.code, false);
                break;

            // @todo Handle Joystick connections/disconnections & button presses
            //		 It is suggested to make joystick button keybinds dynamic with defaults
            //		 that can be saved to disk so that in practice the code
            //		 must not be modified to handle the buttons on the machine.
            //		 Key mappings are OS & Jopystick/controller dependent therefore we
            //		 cannot know with certainty what they will be.
            case sf::Event::JoystickButtonPressed:
            case sf::Event::JoystickMoved:
            case sf::Event::JoystickConnected:
            case sf::Event::JoystickDisconnected:

            default:
                break;
            }
            if (e.type == sf::Event::Closed)
                window.close();
        }

        game.tick(actions, clock.restart().asMilliseconds() / 1000.);

        currentFrame.clear();
        currentFrame.draw(game);
        currentFrame.display();

        smoothShader.setUniform("currentFrame", currentFrame.getTexture());
        smoothShader.setUniform("lastFrame", previousFrame.getTexture());
        smoothShader.setUniform("maxDelta", 0.3f);

        output.clear();
        output.draw(sf::Sprite(currentFrame.getTexture()), &smoothShader);
        output.display();

        window.clear();
        window.draw(sf::Sprite(output.getTexture()));
        window.display();

        previousFrame.clear();
        previousFrame.draw(sf::Sprite(output.getTexture()));
        previousFrame.display();
    }

    delete textures;

    return 0;
}


sf::Vector2u getMaxAspectResolution(int screenWidth,
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

void setAction(Action &actions, sf::Keyboard::Key key, bool pressed)
{
    using Key = sf::Keyboard::Key;
    switch (key)
    {
    case Key::W:		// UP
    case Key::Up:       // UP
        actions.flags.up = pressed;
        break;

    case Key::S:		// DOWN
    case Key::Down:		// DOWN
        actions.flags.down = pressed;
        break;

    case Key::A:		// THRUST
    case Key::Left:		// THRUST
        actions.flags.thrust = pressed;
        break;

    case Key::D:		// CHANGE DIRECTION
        if (pressed)
            actions.flags.left = ~actions.flags.left;
        break;

    case Key::Right:	// CHANGE DIRECTION
        actions.flags.leftHS = pressed;

    case Key::Space:    // FIRE
        actions.flags.fire = pressed;
        break;

    case Key::LShift:   // HYPERSPACE
        actions.flags.hyperspace = pressed;
        break;

    case Key::LControl: // SMART BOMB
        actions.flags.smart_bomb = pressed;
        break;
    }
}

sf::Texture *loadSpritesheet()
{
    auto tex = new sf::Texture;
    tex->loadFromFile("res/Spritesheet.png");

    Animation::setTexture(tex);

    return tex;
}

//         ___   ___   ___
//        /   \ /   \ /   \
//   /\/\|  |  |  |  |  |  |
//  ('w' |  |     |  |  |  |__
//   \_____/ \___/ \___/ \____)
//
//           Inch Cat
