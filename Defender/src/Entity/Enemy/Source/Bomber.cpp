#include "../Bomber.h"

void Bomber::tick(double deltatime)
{
	if (rerollTim)
		rerollTim--; 
	else 
	{
		if (abs(EntityData::PLAYER_REF.pos->x - pos.x) > 300) 
		{
			yMovTimStart = (rand() % 20) + 10;
			range = (rand() % 20) + 5;
			dx = ((rand() % 200) - 100);

			rerollTim = 100;
		}
	}

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

	if (abs(pos.y - EntityData::PLAYER_REF.pos->y) < range)
	{
		stopTim--;

		if (!stopTim) 
		{
			yMovTim = yMovTimStart;
			stopTim = 20;
			vel.y = 0;
		}
	}

	if (pos.y < COMN::uiHeight)
		pos.y = COMN::uiHeight;

	if (pos.y > 240-7)
		pos.y = 240-7;

	vel.x = dx;

	Entity::tick(deltatime);
}