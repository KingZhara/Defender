#include "../Pod.h"

void Pod::tick(double deltatime)
{
	vel.x = (float)(43 * cos(rot*(3.141592653589/180)));
	vel.y = (float)(43 * sin(rot*(3.141592653589/180)));

	if (pos.y < 0)
		pos.y += 255;

	if (pos.y > 255)
		pos.y -= 255;

	Entity::tick(deltatime);
}