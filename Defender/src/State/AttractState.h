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
		switch (stage)
		{
		case 4:
			UserInterface::getShader(ShaderID::HUE_SHIFT)->setUniform("texture", sf::Shader::CurrentTexture);
			UserInterface::getShader(ShaderID::RAND_COL)->setUniform("texture", sf::Shader::CurrentTexture);
			UserInterface::getShader(ShaderID::WILLIAMS)->setUniform("texture", sf::Shader::CurrentTexture);
			[[fallthrough]];

		case 3: // Copyright
			shifting->draw(copyright, states);
			shifting->draw(credits, states);
			[[fallthrough]];

		case 2: // Defender
			UserInterface::getShader(ShaderID::RAND_COL)->setUniform("targetColor", sf::Glsl::Vec3(1.f, 1.f, 0.f));
			flashing->draw(defender, states);
			[[fallthrough]];

		case 1: // Electronics Inc
			shifting->draw(electronicsInc, states);
			shifting->draw(presents, states);
			[[fallthrough]];

		case 0: // Williams
			// Yes, this needs to be here otherwise it will be fully drawn after the first time thru
			willFlashing->clear(sf::Color(0)); 
			willFlashing->draw(williams, states); // Will require alternate shader
		}

		states.shader = UserInterface::getShader(ShaderID::RAND_COL);
		target.draw(flshDra, states);

		// Reset shader target
		UserInterface::getShader(ShaderID::RAND_COL)->setUniform("targetColor", COMN::ShaderTarget);

		states.shader = UserInterface::getShader(ShaderID::HUE_SHIFT);
		target.draw(shftDra, states);

		states.shader = UserInterface::getShader(ShaderID::WILLIAMS);
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
	static sf::Image willImg;
	static sf::Texture* willTex;
	
	unsigned willPos = 0;
	int stage = 0;
	
};

