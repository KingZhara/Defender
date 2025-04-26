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
    UserInterface::initialize();
    DisplayManager::initialize();
    AttractState::initialize();
    HighscoreState::initialize();

    srand((uint32_t)time(nullptr));

    // The game itself
    Game game;
    // Player actions; passed throughout the tick pipeline as special handling is included in AttractState
    Action actions;
    // The Clock used for deltatime
    sf::Clock clock;
    // Min game loop

    Player::setActions(actions);

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

		//actions = Action(); // Reset actions for next frame

        DisplayManager::draw(game);
    }

    DisplayManager::clean();
    UserInterface::clean();
    AttractState::clean();
    HighscoreState::clean();

    return 0;
}

void setAction(Action &actions, sf::Keyboard::Key key, bool pressed)
{

    using Key = sf::Keyboard::Key;
    static std::unordered_set<Key> pressedKeys;

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
        std::cout << "D: " << pressedKeys.count(key) << ", R: " << pressed << ", O: " << (pressed && !pressedKeys.count(key)) << '\n';

        if (pressed && !pressedKeys.count(key))
            actions.flags.left = true;
        else
            actions.flags.left = false;
        break;

    case Key::Right:	// CHANGE DIRECTION
        actions.flags.leftHS = pressed;
        break;

    case Key::Space:    // FIRE
        std::cout << "F: " << pressedKeys.count(key) << ", R: " << pressed << ", O: " << (pressed && !pressedKeys.count(key));
        if (pressed && !pressedKeys.count(key))
            actions.flags.fire = true;
        else
            actions.flags.fire = false;
        std::cout << ", V: " << (short)actions.flags.fire << '\n';
        break;

    case Key::LShift:   // HYPERSPACE
        if (pressed && !pressedKeys.count(key))
            actions.flags.hyperspace = true;
        else
            actions.flags.hyperspace = false;
        break;

    case Key::LControl: // SMART BOMB
        if (pressed && !pressedKeys.count(key))
            actions.flags.smart_bomb = true;
        else
            actions.flags.smart_bomb = false;
        break;
    }

    if (pressed)
        pressedKeys.emplace(key);
    else
        pressedKeys.erase(key);
}

//         ___   ___   ___
//        /   \ /   \ /   \
//   /\/\|  |  |  |  |  |  |
//  ('w' |  |  |  |  |  |  |__
//   \_____/ \___/ \___/ \____)
//
//           Inch Cat

// TODO
// 1. Map wrapping (EntityManager)
// 2. Identify Swarmer direction switching logic
// 3. Particalization (URGENT)
// 4. Stars (Literally anything)
// 5. Scores (Entity Manager -> Stage State somehow)
// 6. UI Elements (User Interface)
// 7. Spawn Logic
