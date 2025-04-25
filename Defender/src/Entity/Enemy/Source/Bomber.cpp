#include "../Bomber.h"

void Bomber::tick(double deltatime)
{
	if(yMovTim)
	{
		if (pos.y < EntityData::PLAYER_REF.pos->y)
			yMovTim--;

		if (pos.y > EntityData::PLAYER_REF.pos->y)
			yMovTim--;
	}
	else 
	{
		if (pos.y < EntityData::PLAYER_REF.pos->y)
			vel.y = 100;

		if (pos.y > EntityData::PLAYER_REF.pos->y)
			vel.y = -100;
	}

	if (pos.y == EntityData::PLAYER_REF.pos->y)
		yMovTim = 10;

	Entity::tick(deltatime);
}