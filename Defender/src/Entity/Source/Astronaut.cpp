#include "../Astronaut.h"

void Astronaut::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	miniSprite->setTextureRect(sf::IntRect(84, 3, 2, 2));

	Entity::draw(target, states);
}

void Astronaut::tick(double deltatime)
{
	// Falling
	if (held == false)
	{
		if (pos.y < COMN::resolution.y - UserInterface::getHeight(pos.x))
		{
			vel.y += (getEVel(ID).y / 5) * deltatime;
			onGround = false;
		}
		// On ground
		else
		{
			vel.y = 0;
			onGround = true;
		}
	}

	Entity::tick(deltatime);
}

void Astronaut::setTargeted(bool v)
{
	isTargeted = v;
}

bool Astronaut::targeted() { return isTargeted && isOnGround(); } // May not be targeted if not on ground
void Astronaut::setHolder(Entity* holder_)
{
	holder = holder_;

	if (holder_ == nullptr)
		held = false;
	else
		held = true;
}
