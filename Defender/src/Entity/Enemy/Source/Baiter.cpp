#include "../Baiter.h"

void Baiter::tick(double deltaTime)
{
	static int16_t destinationX = 0;
	static int16_t destinationY = 0;
	static double slopeX = 0;
	static double slopeY = 0;
	static bool init = true;

	if (init) 
	{
		destinationX = (playerPos->x - pos.x) + playerPos->x;

		destinationY = (playerPos->y - pos.y) + playerPos->y;

		slopeX = (destinationX - pos.x);
		slopeY = (destinationY - pos.y);

		init = false;
	}
	
	vel.x = slopeX / 70;
	vel.y = slopeY / 70;

	Entity::tick(deltaTime);
}