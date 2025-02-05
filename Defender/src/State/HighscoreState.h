#pragma once

#include <SFML/Graphics.hpp>

class HighscoreState : public sf::Drawable
{
public:

	HighscoreState();

	~HighscoreState();

	void tick();

	void draw(const sf::RenderTarget& target, sf::RenderStates states) const
	{

	}
};

