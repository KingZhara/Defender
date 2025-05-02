#include "../Projectile.h"
#include "../Enemy.h"
#include "../Player.h"

#include "../../Utility/common.h"

bool Projectile::collide(Entity *other)
{
	sf::FloatRect bound = sf::FloatRect(other->getPos(), {(float)getBounds(other->getID()).getSize().x, (float)getBounds(other->getID()).getSize().y});

	return Projectile::collide(bound);
}

bool Projectile::collide(sf::FloatRect otherBound)
{
	bool ret = false;

	if (!Entity::collide(otherBound))
	{
		// Move back to the previous position
		animation.setPosition(pos - vel * lastDT);

		// Compute total displacement
		sf::Vector2f delta = vel * lastDT;
		float distance = std::hypot(delta.x, delta.y);

		if (distance == 0.f)
		{
			// No movement; collision must already be present (or not)
			animation.setPosition(pos);
			return false;
		}

		// Determine number of steps (1 every 2 pixels)
		const float stepSize = 2.f;
		uint16_t stepCount = static_cast<uint16_t>(std::ceil(distance / stepSize));

		// Direction per step
		sf::Vector2f step = delta / static_cast<float>(stepCount);

		// Step backwards along the movement path
		for (uint16_t i = 1; i <= stepCount; ++i)
		{
			sf::Vector2f testPos = pos - step * static_cast<float>(i);
			animation.setPosition(testPos);

			if (Entity::collide(otherBound))
			{
				pos = testPos; // Set to point of collision
				ret = true;
				break;
			}
		}

		// Finalize animation position
		animation.setPosition(pos);
	}
	else
	{
		ret = true; // Already colliding, no need to step
	}

	if (ret)
		std::cout << "COLLISION!\n";

	return ret;
}
