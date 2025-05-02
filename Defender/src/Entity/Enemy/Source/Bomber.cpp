#include "../Bomber.h"

void Bomber::tick(double deltatime)
{
	//!!!!!!!!!! ELLIOT READ THIS !!!!!!!!
	// The bomber moves in random directions and is not related to the players position.
	// its similar to the pod, except its direction keeps changing, assume once every 2 seconds.
	// for this use the same code from the pods constructor except wrap it in a timer like inside of the swarmer,
	// that will properly mimic the behavior

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

	Enemy::tick(deltatime);
}