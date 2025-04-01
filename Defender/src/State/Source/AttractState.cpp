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
	williams.setOrigin(williams.getGlobalBounds().getSize() / 2.f);
	williams.setPosition(COMN::resolution.x / 2, 20);


	electronicsInc.setFont(UserInterface::getFont());
	electronicsInc.setFillColor(sf::Color(0));
	electronicsInc.setCharacterSize(15);
	electronicsInc.setString("ELECTRONICS INC.");
	electronicsInc.setOrigin(electronicsInc.getGlobalBounds().getSize() / 2.f);
	electronicsInc.setPosition(COMN::resolution.x / 2, 50);
	
	presents.setFont(UserInterface::getFont());
	presents.setFillColor(sf::Color(0));
	presents.setCharacterSize(15);
	presents.setString("PRESENTS");
	presents.setOrigin(presents.getGlobalBounds().getSize() / 2.f);
	presents.setPosition(COMN::resolution.x / 2, 60);


	copyright.setFont(UserInterface::getFont());
	copyright.setFillColor(sf::Color(0));
	copyright.setCharacterSize(15);
	copyright.setString("COPYRIGHT CR 1980");
	copyright.setOrigin(copyright.getGlobalBounds().getSize() / 2.f);
	copyright.setPosition(COMN::resolution.x / 2, 120);
	
	credits.setFont(UserInterface::getFont());
	credits.setFillColor(sf::Color(0));
	credits.setCharacterSize(15);
	credits.setString("CREDITS: 00");
	credits.setOrigin(credits.getGlobalBounds().getSize() / 2.f);
	credits.setPosition(COMN::resolution.x / 2, 130);
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