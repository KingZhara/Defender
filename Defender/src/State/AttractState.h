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
			UserInterface::getShiftingShader()->setUniform("texture", sf::Shader::CurrentTexture);
			UserInterface::getFlashingShader()->setUniform("texture", sf::Shader::CurrentTexture);
			[[fallthrough]];

		case 3: // Copyright
			shifting->draw(copyright, states);
			shifting->draw(credits, states);
			[[fallthrough]];

		case 2: // Defender
			target.draw(defenderSides, states); // Stays red
			flashing->draw(defenderFront, states);
			[[fallthrough]];

		case 1: // Electronics Inc
			shifting->draw(electronicsInc, states);
			shifting->draw(presents, states);
			[[fallthrough]];

		case 0: // Williams
			target.draw(williams, states); // Will require alternate shader
		}

		states.shader = UserInterface::getFlashingShader();
		target.draw(flshDra, states);


		states.shader = UserInterface::getShiftingShader();
		target.draw(shftDra, states);
	}

private:

	sf::Image willSteps, willImg;
	sf::Texture willTex;
	sf::RectangleShape williams;

	sf::Text electronicsInc, presents, copyright, credits;

	sf::Texture defenderFrontTex, defenderSidesTex;
	sf::RectangleShape defenderFront, defenderSides;

	sf::RenderTexture *flashing, *shifting;

	sf::Sprite flshDra, shftDra;

	unsigned willPos = 0;

	int stage = 0;
	
};

