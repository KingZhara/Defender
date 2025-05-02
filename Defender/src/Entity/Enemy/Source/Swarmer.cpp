#include "../Swarmer.h"

#define epsilon 0.01

void Swarmer::tick(double deltatime)
{

	if (targetTimer.tick(deltatime))
	{
		vel = Entity::makePlayerTargetedVec(pos, ID, 1, true).vel;
		// E
		if (!(rand() % 3))
			vel.y *= -1;
		if (!(rand() % 3))
			vel.x *= -1;

	}

	// Add off screen handling here... this is magic RICKY will handle
	//std::cout << "swarmer pos: ";
	//std::cout << pos.x << ',' << pos.y << '\n';

	Enemy::tick(deltatime);
}
 