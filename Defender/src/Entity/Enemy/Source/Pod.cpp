#include "../Pod.h"

void Pod::tick(double deltatime)
{

	vel.x = (float)(43 * cos(rot*(3.141592653589/180)));
	vel.y = (float)(43 * sin(rot*(3.141592653589/180)));


	Enemy::tick(deltatime);
}