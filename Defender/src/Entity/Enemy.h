#pragma once
#include "Entity.h"
class Enemy :
    public Entity
{
public:
	Enemy(sf::Vector2f pos_, EntityID::ID id_,
		bool         isScripted_ = false, EntityScript* script_ = nullptr)
		: Entity(pos_, id_, isScripted_, script_) {}
};

