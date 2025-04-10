#include "../Baiter.h"
#include<math.h>

void Baiter::tick(double deltaTime)
{
	static int16_t destinationX = 0;
	static int16_t destinationY = 0;
	static bool init = true;
	static double rot;
	sf::Vector2f& playerPos = *Entity::EntityData::PlayerRef::pos;
	sf::Vector2f& playervel = *Entity::EntityData::PlayerRef::pos;

	if (init) 
	{
		destinationX = (uint16_t)(playerPos.x + ((playerPos.x - pos.x) / 3));

		destinationY = (uint16_t)(playerPos.y + ((playerPos.y - pos.y) / 3));

		rot = atan2(playerPos.y - pos.y, playerPos.x - pos.x);

		vel.x = (float)(300 * cos(rot));
		vel.y = (float)(300 * sin(rot));

		init = false;
	}

	//if too far from the player
	if (abs(playerPos.x - pos.x) > 100 || abs(playerPos.y - pos.y) > 100)
	{
		destinationX = playerPos.x;
		destinationY = playerPos.y;

		rot = atan2(playerPos.y - pos.y, playerPos.x - pos.x);

		vel.x = (float)(300 * cos(rot));
		vel.y = (float)(300 * sin(rot));
	}

	if (pos.y < 0)
		pos.y = 0;

	if (vel.x > 0 && pos.x > destinationX)
		init = true;

	if (vel.x < 0 && pos.x < destinationX)
		init = true;

	if (vel.y > 0 && pos.y > destinationY)
		init = true;

	if (vel.y < 0 && pos.y < destinationY)
		init = true;

	Entity::tick(deltaTime);
}