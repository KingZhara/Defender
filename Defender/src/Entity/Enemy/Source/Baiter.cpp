#include "../Baiter.h"
#include<math.h>

void Baiter::tick(double deltaTime)
{
	static int16_t destinationX = 0;
	static int16_t destinationY = 0;
	static double slopeX = 0;
	static double slopeY = 0;
	static bool init = true;
	static double rot;

	if (init) 
	{
		if (playerPos->x > pos.x)
			destinationX = (playerPos->x + abs(pos.x - playerPos->x));
		else
			destinationX = (playerPos->x + (pos.x - playerPos->x));

		if (playerPos->y > pos.y)
			destinationY = (playerPos->y + abs(pos.y - playerPos->y));
		else
			destinationY = (playerPos->y + (pos.y - playerPos->y));


		rot = atan2(playerPos->y - pos.y, playerPos->x - pos.x);

		vel.x = 3 * cos(rot) - 0 * sin(rot);
		vel.y = 3 * sin(rot) + 0 * cos(rot);

		init = false;
	}

	if (vel.x > 0 && pos.x > destinationX)
		init = true;

	if (vel.x < 0 && pos.x < destinationX)
		init = true;

	Entity::tick(deltaTime);
}