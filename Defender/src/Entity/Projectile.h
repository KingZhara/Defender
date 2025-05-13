#pragma once
#include "Entity.h"

class Projectile :
	public Entity
{
public:
	Projectile(sf::Vector2f pos_, EntityID::ID id_,
		bool         isScripted_ = false, EntityScript* script_ = nullptr);

	virtual bool collide(Entity *other) override;
	virtual bool collide(sf::FloatRect otherBound) override;

	virtual void tick(double deltatime) override;

private:
	float lastDT;
};