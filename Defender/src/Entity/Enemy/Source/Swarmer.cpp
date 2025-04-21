#include "../Swarmer.h"

void Swarmer::tick(double deltatime)
{
	static Timer<double> targetTimer{ 1 };

	if (targetTimer.tick(deltatime))
		vel = Entity::makePlayerTargetedVec(pos, 2);
	/*
	if (pos.x < (EntityData::PlayerRef::pos->x - COMN::resolution.x*1.5) || pos.x >(EntityData::PlayerRef::pos->x + COMN::resolution.x*1.5))
		init = true;

	if (pos.y < 0 || pos.y > COMN::resolution.y)
		rot+=180;
	*/
	//vel.x *= 300;
	//vel.y *= 300;
	std::cout << "swarmer pos: ";
	std::cout << pos.x << ',' << pos.y << '\n';

	Entity::tick(deltatime);
}
