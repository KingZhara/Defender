#pragma once

#include <SFML/Graphics.hpp>

#include "../Utility/UserInterface/UserInterface.h"

class AttractState : public sf::Drawable
{
public:

	AttractState();

	static void initialize();

	static void clean()
	{
		delete willTex;
		delete defenderFrontTex;
		delete defenderSidesTex;
		delete flashing;
		delete shifting;
	}

	~AttractState();

	bool tick(double deltatime);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		sf::Texture& fontTex = const_cast<sf::Texture&>(UserInterface::getFont().getTexture(electronicsInc.getCharacterSize()));
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

	static sf::Image willSteps;
	static sf::RectangleShape williams;

	static sf::Text electronicsInc, presents, copyright, credits;

	static sf::Texture *defenderFrontTex, *defenderSidesTex;
	static sf::RectangleShape defenderFront, defenderSides;

	static sf::RenderTexture *flashing, *shifting;

	static sf::Sprite flshDra, shftDra;

	// needs to be recreated each time
	static sf::Image willImg;
	static sf::Texture* willTex;
	
	unsigned willPos = 0;

	int stage = 0;
	
};

