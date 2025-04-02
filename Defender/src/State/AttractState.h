#pragma once

#include <SFML/Graphics.hpp>

#include "../Utility/UserInterface/UserInterface.h"

class AttractState : public sf::Drawable
{
public:

	AttractState();

	~AttractState();

	bool tick(double deltatime);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		switch (stage)
		{
		case 4:

		case 3: // Copyright
			states.shader = UserInterface::getShiftingShader();
			target.draw(copyright, states);
			target.draw(credits, states);
			[[fallthrough]];
		case 2: // Defender
			target.draw(defenderSides, states); // Stays red
			target.draw(defenderFront, states);
			[[fallthrough]];
		case 1: // Electronics Inc
			target.draw(electronicsInc, states);
			target.draw(presents, states);
			[[fallthrough]];
		case 0: // Williams
			target.draw(williams, states); // Will require alternate shader
		}
	}

private:

	sf::Image willSteps, willImg;
	sf::Texture willTex;
	sf::RectangleShape williams;

	sf::Text electronicsInc, presents, copyright, credits;

	sf::Texture defenderFrontTex, defenderSidesTex;
	sf::RectangleShape defenderFront, defenderSides;

	unsigned willPos = 0;

	int stage = 0;
	
};

