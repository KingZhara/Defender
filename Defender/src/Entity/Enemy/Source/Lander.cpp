#include "../Lander.h"

void Lander::tick(double deltatime)
{
	// Ricky will handle logic for picking an astronaut

	// When no astronauts are available, make wander based on a timer; assume can check with "getAstronaut()"; if it returns -1, no astronauts are available
	Enemy::tick(deltatime);
}

bool Lander::hasTarget() { return target; }

void Lander::setTarget(Entity *target_)
{
	target = target_;
}

int Lander::getAstronauts()
{
	return -1;
}
