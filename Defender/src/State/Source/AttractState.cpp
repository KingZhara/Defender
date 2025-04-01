#include "../AttractState.h"
#include "../../Utility/UserInterface/UserInterface.h"
#include "../../Utility/common.h"

#include <iostream>


AttractState::AttractState()
{
	willSteps.loadFromFile("res/williams.png");
	willImg.create(willSteps.getSize().x, willSteps.getSize().y, sf::Color(0));
	willTex.create(willSteps.getSize().x, willSteps.getSize().y);

	williams.setSize(sf::Vector2f(willSteps.getSize().x, willSteps.getSize().y));
	williams.setTexture(&willTex);
	williams.setPosition(COMN::resolution.x / 2 - williams.getGlobalBounds().getSize().x / 2, 10);


	electronicsInc.setFont(UserInterface::getFont());
	electronicsInc.setFillColor(sf::Color(0));
	electronicsInc.setString("ELECTRONICS INC.");
	electronicsInc.setCharacterSize(16);
	electronicsInc.setPosition(COMN::resolution.x / 2 - (18 * 16 / 4), 50);

	presents.setFont(UserInterface::getFont());
	presents.setFillColor(sf::Color(0));
	presents.setCharacterSize(16);
	presents.setString("PRESENTS");
	presents.setPosition(COMN::resolution.x / 2 - (18 * 8 / 4), 60);


	defenderFrontTex.loadFromFile("res/defenderFront.png");
	defenderFront.setSize(sf::Vector2f(defenderFrontTex.getSize()) / 2.f);
	defenderFront.setTexture(&defenderFrontTex);
	defenderFront.setPosition(COMN::resolution.x / 2 - defenderFront.getGlobalBounds().getSize().x / 2, 90);
	defenderFront.setFillColor(sf::Color(0));

	defenderSidesTex.loadFromFile("res/defenderSides.png");
	defenderSides.setSize(sf::Vector2f(defenderSidesTex.getSize()) / 2.f);
	defenderSides.setTexture(&defenderSidesTex);
	defenderSides.setPosition(COMN::resolution.x / 2 - defenderSides.getGlobalBounds().getSize().x / 2, 90);
	defenderSides.setFillColor(sf::Color(0));


	copyright.setFont(UserInterface::getOtherFont());
	copyright.setFillColor(sf::Color(0));
	copyright.setCharacterSize(16);
	copyright.setString("COPYRIGHT @ 1980");
	copyright.setPosition(COMN::resolution.x / 2 - 40, 150);
	
	credits.setFont(UserInterface::getFont());
	credits.setFillColor(sf::Color(0));
	credits.setCharacterSize(16);
	credits.setString("CREDITS: 00");
	credits.setPosition(COMN::resolution.x / 4, 160);
}

AttractState::~AttractState()
{
}

bool AttractState::tick(double deltatime)
{
	static int timer;
	static double deltaTime;
	deltaTime += deltatime;

	while (deltaTime >= 0.04)
	{
		deltaTime -= 0.04;

		if (timer)
		{
			timer--;
		}
		else
		{
			switch (stage)
			{
			case 0: // Williams
				willPos++;
				for (unsigned x = 0; x < willSteps.getSize().x; x++)
					for (unsigned y = 0; y < willSteps.getSize().y; y++)
						if (willSteps.getPixel(x, y).toInteger() < willPos && 
							willSteps.getPixel(x, y).toInteger())
							willImg.setPixel(x, y, sf::Color::White);

				willTex.update(willImg);
				williams.setTexture(&willTex);

				if (willPos > 110)
				{
					willPos = 0;
					stage++;
					timer += 80;
				}

				break;

			case 1: // Electronics Inc presents
				electronicsInc.setFillColor(sf::Color::White);
				presents.setFillColor(sf::Color::White);
				stage++;
				timer += 80;
				break;

			case 2: // Defender
				defenderFront.setFillColor(sf::Color::White);
				defenderSides.setFillColor(sf::Color::White);
				stage++;
				timer += 80;
				break;

			case 3: // Copyright credits
				copyright.setFillColor(sf::Color::White);
				credits.setFillColor(sf::Color::White);
				stage++;
				timer += 80;
				break;

			case 4:
				return true;
			}
		}
	}
	return false;
}