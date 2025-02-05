#pragma once

#include <SFML/Graphics.hpp>

class StageState : public sf::Drawable
{
public:

	StageState();

	~StageState();

	void tick();

	void draw(const sf::RenderTarget& target, sf::RenderStates states) const
	{

	}
};

