#include "../Bomber.h"

Bomber::Bomber(sf::Vector2f pos_, bool isScripted_, EntityScript *script_): Enemy(pos_, EntityID::BOMBER, isScripted_, script_)
{}

void Bomber::tick(double deltatime)
{

	//rerolls the stats

	if (!isOnScreen()) 
	{
		if (rerollTim)
			rerollTim--;
		else
		{
			yMovTimStart = (rand() % 20) + 10;
			range = (rand() % 20) + 5;

			do
			{
				dx = ((rand() % 200) - 100);
			} while (abs(dx) < 20);

			rerollTim = 100;
		}
	}

	//matches the player's y value based on a delay
	if(yMovTim)
	{
		if (pos.y != EntityData::PLAYER_REF.pos->y)
			yMovTim--;
	}
	else 
	{
		if (isOnScreen())
		{
			if (pos.y < EntityData::PLAYER_REF.pos->y)
			{
				if (vel.y < 100)
					vel.y += 10;
			}

			if (pos.y > EntityData::PLAYER_REF.pos->y)
			{
				if (vel.y > -100)
					vel.y -= 10;
			}
		}
	}

	//stops when it's at the player's height
	if (abs(pos.y - EntityData::PLAYER_REF.pos->y) < range && isOnScreen())
	{
		stopTim--;

		if (!stopTim)
		{
			yMovTim = yMovTimStart;
			stopTim = 20;
			vel.y = 0;
		}
	}

	//keeps it from going above or below the screen
	if (pos.y < COMN::uiHeight)
		pos.y = COMN::uiHeight;

	if (pos.y > 240-7)
		pos.y = 240-7;

	vel.x = dx;

	if (attackTimer.tick(deltatime))
		entityQueue.emplace(QueuedEntity(makeCenteredTL(pos, ID), EntityID::BOMB));

	Entity::tick(deltatime);
}
