#include "../Baiter.h"
#include<math.h>

Baiter::Baiter(sf::Vector2f pos_, bool isScripted_, EntityScript *script_): Enemy(pos_, EntityID::BAITER, isScripted_, script_)
{}

void Baiter::tick(double deltatime)
{
	if (targetTimer.tick(deltatime))
		vel = makePlayerTargetedVec(pos, ID, 1).vel;
	// Attack
	if (attackTimer.tick(deltatime))
		entityQueue.emplace(QueuedEntity(pos, EntityID::BULLET));

	Enemy::tick(deltatime);
}
