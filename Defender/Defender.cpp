#include <SFML/Graphics.hpp>

#include "Defender.hpp"

#include "src/Utility/Action.h"

void getAction(Action& actions, sf::Keyboard::Key key);

int main()
{
	sf::RenderWindow window(sf::VideoMode(292, 240), "Defender");

	Game game;
	//GameState state;

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
				getAction(actions, e.key.code);
				break;

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


void getAction(Action& actions, sf::Keyboard::Key key)
{
	
}

//         ___   ___   ___
//        /   \ /   \ /   \
//   /\/\|  |  |  |  |  |  |
//  ('w' |  |     |  |  |  |__
//   \_____/ \___/ \___/ \____)
//
//           Inch Cat
