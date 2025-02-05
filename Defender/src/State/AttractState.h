#pragma once

#include <SFML/Graphics.hpp>

class AttractState : public sf::Drawable
{
public:

	AttractState();

	~AttractState();

	void tick();

	void draw(const sf::RenderTarget& target, sf::RenderStates states) const
	{

	}
};

