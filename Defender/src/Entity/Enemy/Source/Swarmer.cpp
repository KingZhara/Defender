#include "../Swarmer.h"

void Swarmer::tick(double deltatime)
{
	if (init) 
	{
		rot = atan2(EntityData::PlayerRef::pos->y - pos.y, EntityData::PlayerRef::pos->x - pos.x);

		init = false;
	}

	if (pos.x < (EntityData::PlayerRef::pos->x - COMN::resolution.x*1.5) || pos.x >(EntityData::PlayerRef::pos->x + COMN::resolution.x*1.5))
		init = true;

	if (pos.y < 0 || pos.y > COMN::resolution.y)
		rot+=180;

	vel.x = (float)(300 * cos(rot));
	vel.y = (float)(300 * sin(rot));

	Entity::tick(deltatime);
}
