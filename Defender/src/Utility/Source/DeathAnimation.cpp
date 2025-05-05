#include "../DeathAnimation.h"
void DeathAnimation::tick(double deltatime)
{
	for (int i = 0; i < 500; i++)
	{
		pieces[i].tick(deltatime);
	}
}

void DeathAnimation::draw(sf::RenderTarget &target,
        sf::RenderStates states) const
{
	states.shader = UserInterface::getShader(ShaderID::RAND_COL);
	for (int i = 0; i < 500; i++)
	{
		pieces[i].draw(target, states);
	}
}
