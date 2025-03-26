#include <SFML/Graphics.hpp>
#include "Defender.hpp"
#include "src/Utility/DisplayManager.h"
#include "src/Utility/Action.h"
#include "src/Utility/common.h"

void         setAction(Action &actions, sf::Keyboard::Key key, bool pressed);
sf::Texture *loadSpritesheet();
sf::Vector2u getMaxAspectResolution(int screenWidth,
                                    int screenHeight,
                                    int aspectWidth,
                                    int aspectHeight);

int main()
{
    // The game itself
    Game game;
    // Player actions; passed throughout the tick pipeline as special handling is included in AttractState
    Action actions;
    // The Clock used for deltatime
    sf::Clock clock;

    UserInterface::initialize();
    DisplayManager::initialize();

    // Min game loop
    while (DisplayManager::getWindow()->isOpen())
    {
        // The windows events
        sf::Event e;
        while (DisplayManager::getWindow()->pollEvent(e))
        {
            switch (e.type)
            {
            case sf::Event::Closed:
                DisplayManager::getWindow()->close();
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
        }

        game.tick(actions, clock.restart().asMilliseconds() / 1000.);

        DisplayManager::getRenderTarget()->draw(game);
        DisplayManager::draw();
    }

    return 0;
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

//         ___   ___   ___
//        /   \ /   \ /   \
//   /\/\|  |  |  |  |  |  |
//  ('w' |  |     |  |  |  |__
//   \_____/ \___/ \___/ \____)
//
//           Inch Cat
