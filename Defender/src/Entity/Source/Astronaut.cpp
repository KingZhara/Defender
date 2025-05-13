#include "../Astronaut.h"

Astronaut::Astronaut(sf::Vector2f pos_, bool isScripted_, EntityScript *script_): Entity(pos_, EntityID::ASTRONAUT, isScripted_, script_), onGround(true), held(false)
{
	startFallHeight = pos.y;
}

void Astronaut::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	miniSprite->setTextureRect(sf::IntRect(84, 3, 2, 2));

	Entity::draw(target, states);
}

void Astronaut::tick(double deltatime)
{
	vel.x = 0;
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

bool Astronaut::targeted() { return isTargeted; }
bool Astronaut::isOnGround() const { return onGround; }

void Astronaut::setHolder(Entity* holder_)
{
	if (holder_ == nullptr)
	{
		held = false;
		startFallHeight = pos.y;
	}
	else
		held = true;
}

bool Astronaut::shouldDie()
{
    return onGround && abs(pos.y - startFallHeight > COMN::resolution.y / 4);
}
