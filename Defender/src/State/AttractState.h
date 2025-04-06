#pragma once

#include <SFML/Graphics.hpp>

#include "../Utility/UserInterface/UserInterface.h"
#include "../Utility/common.h"

class AttractState : public sf::Drawable
{
public:

	AttractState();

	static void initialize();

	static void clean()
	{
		delete willTex;
		delete defenderTex;
		delete flashing;
		delete shifting;
		delete willFlashing;
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
			UserInterface::getWilliamsShader()->setUniform("texture", sf::Shader::CurrentTexture);
			[[fallthrough]];

		case 3: // Copyright
			shifting->draw(copyright, states);
			shifting->draw(credits, states);
			[[fallthrough]];

		case 2: // Defender
			UserInterface::getFlashingShader()->setUniform("targetColor", sf::Glsl::Vec3(1.f, 1.f, 0.f));
			flashing->draw(defender, states);
			[[fallthrough]];

		case 1: // Electronics Inc
			shifting->draw(electronicsInc, states);
			shifting->draw(presents, states);
			[[fallthrough]];

		case 0: // Williams
			target.draw(williams, states); // Draws normally
			// This below is off by 1 for width and doesn't clear for some reason.
			// I thought I copied the other shaders in every use.
			
			//willFlashing->draw(williams, states); // Will require alternate shader
		}

		states.shader = UserInterface::getFlashingShader();
		target.draw(flshDra, states);
		UserInterface::getFlashingShader()->setUniform("targetColor", COMN::ShaderTarget);


		states.shader = UserInterface::getShiftingShader();
		target.draw(shftDra, states);

		states.shader = UserInterface::getWilliamsShader();
		target.draw(willFlshDra, states);
	}

private:

	static sf::Image willSteps;
	static sf::RectangleShape williams;

	static sf::Text electronicsInc, presents, copyright, credits;

	static sf::Texture *defenderTex;
	static sf::RectangleShape defender;

	static sf::RenderTexture *flashing, *shifting, *willFlashing;

	static sf::Sprite flshDra, shftDra, willFlshDra;

	// needs to be recreated each time
	static sf::Image willImg;
	static sf::Texture* willTex;
	
	unsigned willPos = 0;

	int stage = 0;
	
};

