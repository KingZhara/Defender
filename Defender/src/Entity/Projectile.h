#pragma once
#include "Entity.h"

class Projectile :
	public Entity
{
public:
	Projectile(sf::Vector2f pos_, EntityID::EntityID id_,
		bool         isScripted_ = false, EntityScript* script_ = nullptr)
		: Entity(pos_, id_, isScripted_, script_) {}
};

