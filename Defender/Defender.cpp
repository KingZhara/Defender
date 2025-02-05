#include <SFML/Graphics.hpp>

#include "Defender.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(292, 240), "Defender");

	Game game;

	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
			if (e.type == sf::Event::Closed)
				window.close();

		game.tick();

		window.clear();
		game.draw(window);
		window.display();
	}

	return 0;
}

//         ___   ___   ___
//        /   \ /   \ /   \
//   /\/\|  |  |  |  |  |  |
//  ('w' |  |     |  |  |  |__
//   \_____/ \___/ \___/ \____)
//
//           Inch Cat
