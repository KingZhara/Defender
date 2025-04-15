#include "../Pod.h"

void Pod::tick(double deltatime)
{
	if (rot > 180)
		rot -= 360;

	if (rot < -45 && rot > -135)
		rot = 45;

	if (rot > 45 && rot < 135)
		rot = 135;

	std::cout << rot << '\n';

	vel.x = (float)(43 * cos(rot*(3.141592653589/180)));
	vel.y = (float)(43 * sin(rot*(3.141592653589/180)));

	if (pos.y < 0)
		pos.y += 255;

	if (pos.y > 255)
		pos.y -= 255;

	Entity::tick(deltatime);
}