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
			destinationX = (uint16_t)(playerPos->x + (playerPos->x - pos.x)/2);
			destinationY = (uint16_t)(playerPos->y + (playerPos->y - pos.y)/2);


		rot = atan2(playerPos->y - pos.y, playerPos->x - pos.x);

		vel.x = (float)(3 * cos(rot) - 0 * sin(rot));
		vel.y = (float)(3 * sin(rot) + 0 * cos(rot));

		init = false;
	}

	if (vel.x > 0 && pos.x > destinationX)
		init = true;

	if (vel.x < 0 && pos.x < destinationX)
		init = true;

	Entity::tick(deltaTime);
}