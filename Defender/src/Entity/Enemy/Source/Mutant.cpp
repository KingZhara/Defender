#include "../Mutant.h"

void Mutant::tick(double deltaTime)
{
	double evenCheck;

	if (abs(playerPos->x - pos.x) < 70)
	{
		attack++;
		evenCheck = attack;
		if (attack == 1)
			attackPos = pos.x;
		if (attack == 255)
			attack = 0;

		if ((evenCheck / 5) > (attack / 5))
			vel.x = 0.5f;
		else
			vel.x = -0.5f;

		if (playerPos->y < pos.y)
			vel.y = -0.5f;
		if (playerPos->y > pos.y)
			vel.y = 0.5f;
	}
	else
	{
		attack = 0;

		vel.x = 0.5f;
		vel.y = 0;
	}

	return;
}

void Mutant::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

}