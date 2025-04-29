#include "../Bomber.h"

void Bomber::tick(double deltatime)
{
	if(yMovTim)
	{
		if (pos.y != EntityData::PLAYER_REF.pos->y)
			yMovTim--;
	}
	else 
	{
		if (pos.y < EntityData::PLAYER_REF.pos->y) 
		{
			if (vel.y < 100)
				vel.y += 10;
		}

		if (pos.y > EntityData::PLAYER_REF.pos->y)
		{
			if (vel.y > -100)
				vel.y -= 10;
		}
	}

	if (abs(pos.y - EntityData::PLAYER_REF.pos->y) < 10)
	{
		stopTim--;

		if (!stopTim) 
		{
			yMovTim = 20;
			stopTim = 20;
			vel.y = 0;
		}
	}

	Entity::tick(deltatime);
}