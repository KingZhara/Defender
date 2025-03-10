#include "../Enemy.h"

void Enemy::tick(double deltatime)
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
