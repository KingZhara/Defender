#include <SFML/Graphics.hpp>

#include "Defender.hpp"

#include "src/Utility/Action.h"

void setAction(Action& actions, sf::Keyboard::Key key);

int main()
{
	sf::RenderWindow window(sf::VideoMode(292, 240), "Defender");

	Game game;

	// Player actions; passed throughout the tick pipeline as special handling is included in AttractState
	Action actions;

	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			switch (e.type)
			{
			case sf::Event::Closed:
				break;

			case sf::Event::KeyPressed:
				setAction(actions, e.key.code);
				break;

			// @todo Handle Joystick connections/disconnections & button presses
			//		 It is suggested to make joystick button keybinds dynamic with defaults
			//		 that can be saved to disk so that in practice the code
			//		 must not be modified to handle the buttons on the machine.
			//		 Key mappings are OS & Jopystick/controller dependent therefore we
			//		 cannot know with certainty what they will be.
			case sf::Event::JoystickButtonPressed:
			case sf::Event::JoystickConnected:
			case sf::Event::JoystickDisconnected:

			default:
				break;
			}
			if (e.type == sf::Event::Closed)
				window.close();
		}

		game.tick(actions);

		window.clear();
		window.draw(game);
		//window.draw(state);
		window.display();
	}

	return 0;
}


void setAction(Action& actions, sf::Keyboard::Key key)
{
	using Key = sf::Keyboard::Key;
	switch (key)
	{
	case Key::Up:       // THRUST
	case Key::W:
		actions.flags |= 0b00001000;
		break;

	case Key::Left:     // LEFT
	case Key::A:
		actions.flags |= 0b00100000;
		break;

	case Key::Right:    // RIGHT
	case Key::D:
		actions.flags |= 0b00010000;
		break;

	case Key::Space:    // FIRE
		actions.flags |= 0b00000100;
		break;

	case Key::LShift:   // HYPERSPACE
		actions.flags |= 0b00000001;
		break;

	case Key::LControl: // SMART BOMB
		actions.flags |= 0b00000010;
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
