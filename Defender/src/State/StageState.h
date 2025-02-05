#pragma once

#include <SFML/Graphics.hpp>
#include "../Utility/Action.h"

class StageState : public sf::Drawable
{
public:

	StageState();

	~StageState();

	bool tick(Action& actions);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{

	}
};
