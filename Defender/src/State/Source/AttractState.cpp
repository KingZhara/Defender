#include "../AttractState.h"
#include "../../Utility/UserInterface/UserInterface.h"
#include "../../Utility/common.h"
#include "../../Utility/Timer.h"

#include <iostream>

#include "../../Utility/DisplayManager.h"


AttractState::AttractState()
{
	std::cout << "CONSTR\n";

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

	flashing = new sf::RenderTexture;
	shifting = new sf::RenderTexture;

	flashing->create(COMN::resolution.x, COMN::resolution.y);
	shifting->create(COMN::resolution.x, COMN::resolution.y);

	flashing->setView(DisplayManager::getView());
	shifting->setView(DisplayManager::getView());

	flshDra.setTexture(flashing->getTexture());
	shftDra.setTexture(shifting->getTexture());

	flshDra.setScale(1.f, -1.f);
	shftDra.setScale(1.f, -1.f);

	flshDra.move(0, COMN::resolution.y);
	shftDra.move(0, COMN::resolution.y);
}

AttractState::~AttractState()
{
}

bool AttractState::tick(double deltatime)
{
	static Timer<double> textTimer{ 0.03 };
	static Timer<int> stageTimer{80, false};
	static sf::Clock timer;

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

				willTex.update(willImg);
				williams.setTexture(&willTex);

				if (willPos > 110)
				{
					willPos = 0;
					stage++;
					stageTimer.tick(1); // Will reset
				}

				break;

			case 1: // Electronics Inc presents
				timer.restart();
				electronicsInc.setFillColor(sf::Color(COMN::ShaderTarget));
				presents.setFillColor(COMN::ShaderTarget);
				stage++;
				stageTimer.tick(1);
				break;

			case 2: // Defender
				defenderFront.setFillColor(COMN::ShaderTarget);
				defenderSides.setFillColor(sf::Color(255, 0, 0));
				stage++;
				stageTimer.tick(1);
				break;

			case 3: // Copyright credits
				copyright.setFillColor(COMN::ShaderTarget);
				credits.setFillColor(COMN::ShaderTarget);
				stage++;
				stageTimer.addTime((1 / textTimer.getBase()) * 5);
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