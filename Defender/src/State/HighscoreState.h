#pragma once

#include <SFML/Graphics.hpp>

class HighscoreState : public sf::Drawable
{
public:

	HighscoreState();

	~HighscoreState();

	bool tick(double deltatime);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.clear(sf::Color::Red);
	}
};

