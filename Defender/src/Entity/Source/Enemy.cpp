#include "../Enemy.h"
/*
void Enemy::tick(double deltatime)
{
	if (getID() == EntityID::MUTANT) 
	{
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
	/*
	Entity::entityQueue.emplace(
		QueuedEntity{
			pos,
			atan2(playerPos->y - pos.y, playerPos->x - pos.x),
			EntityID::BOMB
		}
	);
	
}*/
void Enemy::tick(double deltatime)
{
    Entity::tick(deltatime);

	if (pos.y > COMN::playHeight::max)
		pos.y -= COMN::playHeight::max - COMN::playHeight::min;
	else if (pos.y < COMN::playHeight::min)
		pos.y += COMN::playHeight::max - COMN::playHeight::min;
}
