#include "../AttractState.h"


AttractState::AttractState()
{
	willSteps.loadFromFile("res/williams.png");
	willImg.create(willSteps.getSize().x, willSteps.getSize().y, sf::Color(0));

	willTex.create(willSteps.getSize().x, willSteps.getSize().y);

	williams.setSize(sf::Vector2f(willSteps.getSize().x, willSteps.getSize().y));
	williams.setTexture(&willTex);
}

AttractState::~AttractState()
{
}

bool AttractState::tick(double deltatime)
{
	static double deltaTime;
	deltaTime += deltatime;

	while (deltaTime >= 0.045)
	{
		deltaTime -= 0.045;

		switch (stage)
		{
		case 0: // Williams
			willPos++;
			for (int x = 0; x < willSteps.getSize().x; x++)
				for (int y = 0; y < willSteps.getSize().y; y++)
					if (willSteps.getPixel(x, y).toInteger() < willPos && willSteps.getPixel(x, y).toInteger())
						willImg.setPixel(x, y, sf::Color::White);

			willTex.update(willImg);
			williams.setTexture(&willTex);
		}
	}
	return false;
}