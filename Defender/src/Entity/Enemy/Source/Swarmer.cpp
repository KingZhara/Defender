#include "../Swarmer.h"

void Swarmer::tick(double deltatime)
{
	static Timer<double> targetTimer{ 1 };

	if (targetTimer.tick(deltatime))
	{
		vel = Entity::makePlayerTargetedVec(pos, ID, 2).vel;
	}

	// Add off screen handling here... this is magic RICKY will handle
	//std::cout << "swarmer pos: ";
	//std::cout << pos.x << ',' << pos.y << '\n';

	Enemy::tick(deltatime);
}
