#include "../Baiter.h"
#include<math.h>

void Baiter::tick(double deltatime)
{

	//@todo change baiter targeting
	/*
	if (init) 
	{
		destinationX = (uint16_t)(EntityData::PlayerRef::pos->x + ((EntityData::PlayerRef::pos->x - pos.x) / 3));
		destinationY = (uint16_t)(EntityData::PlayerRef::pos->y + ((EntityData::PlayerRef::pos->y - pos.y) / 3));

		rot = atan2(EntityData::PlayerRef::pos->y - pos.y, EntityData::PlayerRef::pos->x - pos.x);

		vel.x = (float)(300 * cos(rot));
		vel.y = (float)(300 * sin(rot));

		init = false;
	}

	//if too far from the player
	if (abs(EntityData::PlayerRef::pos->x - pos.x) > 100 || abs(EntityData::PlayerRef::pos->y - pos.y) > 100)
	{
		destinationX = (uint16_t)EntityData::PlayerRef::pos->x;
		destinationY = (uint16_t)EntityData::PlayerRef::pos->y;

		rot = atan2(EntityData::PlayerRef::pos->y - pos.y, EntityData::PlayerRef::pos->x - pos.x);

		vel.x = (float)(300 * cos(rot));
		vel.y = (float)(300 * sin(rot));
	}

	if (pos.y < 0)
		pos.y = 0;

	if (vel.x > 0 && pos.x > destinationX)
		init = true;

	if (vel.x < 0 && pos.x < destinationX)
		init = true;

	if (vel.y > 0 && pos.y > destinationY)
		init = true;

	if (vel.y < 0 && pos.y < destinationY)
		init = true;
        */
	if (targetTimer.tick(deltatime))
		vel = makePlayerTargetedVec(pos, ID, 1).vel;
	// Attack
	if (attackTimer.tick(deltatime))
		entityQueue.emplace(QueuedEntity(pos, EntityID::BULLET));

	Enemy::tick(deltatime);
}