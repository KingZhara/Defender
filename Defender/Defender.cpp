#include <SFML/Graphics.hpp>

#include "Defender.hpp"

#include "src/Utility/Action.h"

void setAction(Action& actions, sf::Keyboard::Key key);
void loadSpritesheet();
sf::Vector2u getMaxAspectResolution(int screenWidth, int screenHeight, int aspectWidth, int aspectHeight);

int main()
{
	// 73 : 60
	sf::Vector2u resolution = getMaxAspectResolution(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height, 73, 60);

	sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Defender");
	Game game;
	// Player actions; passed throughout the tick pipeline as special handling is included in AttractState
	Action actions;

	sf::View viewport = sf::View(sf::FloatRect(0, 0, 292, 240));

	sf::Clock clock;

	loadSpritesheet();

	StageState::setView(window, viewport);

	Timer<double> timer = Timer<double>(1.);


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

		game.tick(actions, clock.restart().asMilliseconds() / 1000.);

		window.clear();
		window.draw(game);
		//window.draw(state);
		window.display();
	}

	return 0;
}


sf::Vector2u getMaxAspectResolution(int screenWidth, int screenHeight, int aspectWidth, int aspectHeight) {
	sf::Vector2u resolution;

	// Compute width-first scaling
	int heightForMaxWidth = (screenWidth * aspectHeight) / aspectWidth;

	if (heightForMaxWidth <= screenHeight) {
		// The width-first scaling fits
		resolution.x = screenWidth;
		resolution.y = heightForMaxWidth;
	}
	else {
		// The height-first scaling is needed
		resolution.x = (screenHeight * aspectWidth) / aspectHeight;
		resolution.y = screenHeight;
	}

	return resolution;
}

void setAction(Action& actions, sf::Keyboard::Key key)
{
	using Key = sf::Keyboard::Key;
	switch (key)
	{
	case Key::Up:       // THRUST
	case Key::W:
		actions.flags.thrust = true;
		break;

	case Key::Left:     // LEFT
	case Key::A:
		actions.flags.left = true;
		break;

	case Key::Right:    // RIGHT
	case Key::D:
		actions.flags.right = true;
		break;

	case Key::Space:    // FIRE
		actions.flags.fire = true;
		break;

	case Key::LShift:   // HYPERSPACE
		actions.flags.hyperspace = true;
		break;

	case Key::LControl: // SMART BOMB
		actions.flags.smart_bomb = true;
		break;

	}
}

void loadSpritesheet()
{
	sf::Texture tex;
	tex.loadFromFile("res/Spritesheet.png");

	Animation::setTexture(tex);
}

//         ___   ___   ___
//        /   \ /   \ /   \
//   /\/\|  |  |  |  |  |  |
//  ('w' |  |     |  |  |  |__
//   \_____/ \___/ \___/ \____)
//
//           Inch Cat
