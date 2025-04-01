#include "../AttractState.h"
#include "../../Utility/UserInterface/UserInterface.h"
#include "../../Utility/common.h"


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
	electronicsInc.setCharacterSize(12);
	electronicsInc.setString("ELECTRONICS INC.");
	electronicsInc.setPosition(COMN::resolution.x * 0.3, 50);
	
	presents.setFont(UserInterface::getFont());
	presents.setFillColor(sf::Color(0));
	presents.setCharacterSize(12);
	presents.setString("PRESENTS");
	presents.setPosition(COMN::resolution.x * 0.4, 60);


	defenderTex.loadFromFile("res/defender.png");
	defender.setSize(sf::Vector2f(defenderTex.getSize()) / 2.f);
	defender.setTexture(&defenderTex);
	defender.setPosition(COMN::resolution.x / 2 - defender.getGlobalBounds().getSize().x / 2, 90);
	defender.setFillColor(sf::Color(0));


	// This uses a different font, fix later
	copyright.setFont(UserInterface::getFont());
	copyright.setFillColor(sf::Color(0));
	copyright.setCharacterSize(10);
	copyright.setString("COPYRIGHT CR 1980");
	copyright.setPosition(COMN::resolution.x * 0.35, 150);
	
	credits.setFont(UserInterface::getFont());
	credits.setFillColor(sf::Color(0));
	credits.setCharacterSize(12);
	credits.setString("CREDITS: 00");
	credits.setPosition(COMN::resolution.x * 0.3, 160);
}

AttractState::~AttractState()
{
}

bool AttractState::tick(double deltatime)
{
	static int timer;
	static double deltaTime;
	deltaTime += deltatime;

	while (deltaTime >= 0.045)
	{
		deltaTime -= 0.045;

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
				for (int x = 0; x < willSteps.getSize().x; x++)
					for (int y = 0; y < willSteps.getSize().y; y++)
						if (willSteps.getPixel(x, y).toInteger() < willPos && 
							willSteps.getPixel(x, y).toInteger())
							willImg.setPixel(x, y, sf::Color::White);

				willTex.update(willImg);
				williams.setTexture(&willTex);

				if (willPos > 110)
				{
					stage++;
					timer += 100;
				}

				break;

			case 1: // Electronics Inc presents
				electronicsInc.setFillColor(sf::Color::White);
				presents.setFillColor(sf::Color::White);
				stage++;
				timer += 100;
				break;

			case 2: // Defender
				defender.setFillColor(sf::Color::White);
				stage++;
				timer += 100;
				break;

			case 3: // Copyright credits
				copyright.setFillColor(sf::Color::White);
				credits.setFillColor(sf::Color::White);
				stage++;
				timer += 100;
				break;

			//case 4:
				//return true;
			}
		}
	}
	return false;
}