#include "../Mutant.h"

void Mutant::tick(double deltaTime)
{
	if (abs(playerPos->x - pos.x) < 70)
	{
		attack++;
		if (attack == 1)
			attackPos = pos.x;
		if (attack == 255)
			attack = 2;

		if (pos.x > attackPos && pos.x < (attackPos + 5))
			vel.x = 0.5f;

		if (pos.x <= attackPos && pos.x > (attackPos - 5))
			vel.x = -0.5f;

		if (playerPos->y < pos.y)
			vel.y = -0.5f;
		if (playerPos->y > pos.y)
			vel.y = 0.5f;
	}
	else
	{
		attack = 0;

		//vel.x = 0.5f;
		vel.y = 0;
	}
	Entity::tick(deltaTime);
	return;
}