#pragma once

#include <SFML/Graphics.hpp>

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

		case 3: // Copyright
			target.draw(copyright, states);
			target.draw(credits, states);
		case 2: // Defender
			target.draw(defender, states);

		case 1: // Electronics Inc
			target.draw(electronicsInc, states);
			target.draw(presents, states);
		case 0: // Williams
			target.draw(williams, states);
		}
	}

private:

	sf::Image willSteps, willImg;
	sf::Texture willTex;
	sf::RectangleShape williams;

	sf::Text electronicsInc, presents, copyright, credits;

	sf::Texture defenderTex;
	sf::RectangleShape defender;

	int willPos = 0;

	int stage = 0;
	
};

