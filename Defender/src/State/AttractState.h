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
		case 3: // Copyright
		case 2: // Defender
		case 1: // Electronics Inc
		case 0: // Williams
			
			target.draw(williams, states);
		}
	}

private:

	sf::Image willSteps, willImg;
	sf::Texture willTex;
	sf::RectangleShape williams;

	int willPos = 0;

	int stage = 0;
	
};

