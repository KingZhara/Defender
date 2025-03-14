#include "../Mutant.h"

void Mutant::tick(double deltaTime)
{
	if (abs(playerPos->x - pos.x) < 70)
	{
		if (attack == 0)
			attackPos = pos.x;
		if (attack == 255)
			attack = 0;
		attack++;

		if (playerPos->y < pos.y)
			vel.y = -0.5f;
		if (playerPos->y > pos.y)
			vel.y = 0.5f;
	}
	else
	{
		vel.x = 0.5f;
		vel.y = 0;
	}
}

void Mutant::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

}