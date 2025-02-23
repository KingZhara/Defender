#pragma once
#include "Entity.h"
class Enemy :
    public Entity
{
public:
	Enemy(sf::Vector2f pos_,
		bool         isScripted_ = false, EntityScript* script_ = nullptr)
		: Entity(pos_, EntityID::PLAYER, isScripted_, script_) {}

};

