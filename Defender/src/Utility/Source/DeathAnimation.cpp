#include "../DeathAnimation.h"
void DeathAnimation::tick(double deltatime)
{
	for (int i = 0; i < 100; i++)
	{
		pieces[i].tick(deltatime);
	}
}

void DeathAnimation::draw(sf::RenderTarget &target,
        sf::RenderStates states) const
{
	for (int i = 0; i < 100; i++)
	{
		pieces[i].draw(target, states);
	}
}
