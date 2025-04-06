#include "../AttractState.h"
#include "../../Utility/UserInterface/UserInterface.h"
#include "../../Utility/common.h"
#include "../../Utility/Timer.h"

#include <iostream>

#include "../../Utility/DisplayManager.h"


sf::Image AttractState::willSteps, AttractState::willImg;
sf::Texture* AttractState::willTex;
sf::RectangleShape AttractState::williams;

sf::Text AttractState::electronicsInc, AttractState::presents, 
AttractState::copyright, AttractState::credits;

sf::Texture* AttractState::defenderTex;
sf::RectangleShape AttractState::defender;

sf::RenderTexture* AttractState::flashing, * AttractState::shifting, * AttractState::willFlashing;

sf::Sprite AttractState::flshDra, AttractState::shftDra, AttractState::willFlshDra;


AttractState::AttractState()
{
	electronicsInc.setFillColor(sf::Color(0));
	presents.setFillColor(sf::Color(0));
	defender.setFillColor(sf::Color(0));
	copyright.setFillColor(sf::Color(0));
	credits.setFillColor(sf::Color(0));

	willImg.create(willSteps.getSize().x, willSteps.getSize().y, sf::Color(0));

	shifting->clear(sf::Color(0));
	flashing->clear(sf::Color(0));
	willFlashing->clear(sf::Color(0));
}

void AttractState::initialize()
{
	willTex = new sf::Texture;
	defenderTex = new sf::Texture;
	flashing = new sf::RenderTexture;
	shifting = new sf::RenderTexture;
	willFlashing = new sf::RenderTexture;

	willSteps.loadFromFile("res/williams.png");

	williams.setSize(sf::Vector2f((float)willSteps.getSize().x, (float)willSteps.getSize().y));
	williams.setPosition(COMN::resolution.x / 2 - williams.getGlobalBounds().getSize().x / 2, 10);

	willImg.create(willSteps.getSize().x, willSteps.getSize().y, sf::Color(0));
	willTex->create(willSteps.getSize().x, willSteps.getSize().y);

	electronicsInc.setFont(UserInterface::getFont());
	electronicsInc.setString("ELECTRONICS INC.");
	electronicsInc.setCharacterSize(16);
	electronicsInc.setPosition(COMN::resolution.x / 2 - (18 * 16 / 4), 50);

	presents.setFont(UserInterface::getFont());
	presents.setCharacterSize(16);
	presents.setString("PRESENTS");
	presents.setPosition(COMN::resolution.x / 2 - (18 * 8 / 4), 60);


	defenderTex->loadFromFile("res/defender.png");
	defender.setSize(sf::Vector2f(defenderTex->getSize()) / 2.f);
	defender.setTexture(defenderTex);
	defender.setPosition(COMN::resolution.x / 2 - defender.getGlobalBounds().getSize().x / 2, 90);


	copyright.setFont(UserInterface::getOtherFont());
	copyright.setCharacterSize(16);
	copyright.setString("COPYRIGHT @ 1980");
	copyright.setPosition(COMN::resolution.x / 2 - 40, 150);

	credits.setFont(UserInterface::getFont());
	credits.setCharacterSize(16);
	credits.setString("CREDITS: 00");
	credits.setPosition(COMN::resolution.x / 4, 160);

	flashing->create((unsigned)COMN::resolution.x, (unsigned)COMN::resolution.y);
	shifting->create((unsigned)COMN::resolution.x, (unsigned)COMN::resolution.y);
	willFlashing->create((unsigned)COMN::resolution.x, (unsigned)COMN::resolution.y);

	flashing->setView(DisplayManager::getView());
	shifting->setView(DisplayManager::getView());
	willFlashing->setView(DisplayManager::getView());

	flshDra.setTexture(flashing->getTexture());
	shftDra.setTexture(shifting->getTexture());
	willFlshDra.setTexture(willFlashing->getTexture());

	flshDra.setScale(1.f, -1.f);
	shftDra.setScale(1.f, -1.f);
	willFlshDra.setScale(1.f, -1.f);

	flshDra.move(0, COMN::resolution.y);
	shftDra.move(0, COMN::resolution.y);
	willFlshDra.move(0, COMN::resolution.y);
}


AttractState::~AttractState()
{
}

bool AttractState::tick(double deltatime)
{
	static Timer<double> textTimer{ 0.03 };
	static Timer<int> stageTimer{80, false};

	while (textTimer.tick(deltatime))
	{
		if (!stageTimer.isComplete())
		{
			//std::cout << stage << '\n';
			stageTimer.tick(1);
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
							willImg.setPixel(x, y, COMN::ShaderTarget);

				willTex->update(willImg);
				williams.setTexture(willTex);

				if (willPos > 110)
				{
					willPos = 0;
					stage++;
					stageTimer.tick(1); // Will reset
				}

				break;

			case 1: // Electronics Inc presents
				electronicsInc.setFillColor(sf::Color(COMN::ShaderTarget));
				presents.setFillColor(COMN::ShaderTarget);
				stage++;
				stageTimer.tick(1);
				break;

			case 2: // Defender
				defender.setFillColor(sf::Color::White);
				stage++;
				stageTimer.tick(1);
				break;

			case 3: // Copyright credits
				copyright.setFillColor(COMN::ShaderTarget);
				credits.setFillColor(COMN::ShaderTarget);
				stage++;
				stageTimer.addTime((int)(1. / textTimer.getBase() * 5.));
				stageTimer.tick(1);
				break;

			case 4:
				return true;
			}
		}
		deltatime = 0;
	}
	return false;
}