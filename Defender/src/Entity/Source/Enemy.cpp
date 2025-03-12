#include "../Enemy.h"

void Enemy::tick(double deltatime)
{
	bool attackMode = false;

	if (getID() == EntityID::MUTANT) 
	{
		if (abs(playerPos->x-pos.x)<70) 
		{
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

		Entity::tick(deltatime);
	}

	if (getID() == EntityID::LANDER)
	{
		if (playerPos->x < pos.x)
			vel.x = -0.5f;
		else if (playerPos->x > pos.x)
			vel.x = 0.5f;

		if (playerPos->y < pos.y)
			vel.y = -0.5f;
		else if (playerPos->y > pos.y)
			vel.y = 0.5f;

		Entity::tick(deltatime);
	}
}
