#pragma once

#include <SFML/Graphics.hpp>

class AttractState : public sf::Drawable
{
public:

	AttractState();

	~AttractState();

	bool tick(double deltatime);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{

	}
};

