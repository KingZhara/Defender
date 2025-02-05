#pragma once

#include <SFML/Graphics.hpp>

class HighscoreState : public sf::Drawable
{
public:

	HighscoreState();

	~HighscoreState();

	void tick();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{

	}
};

