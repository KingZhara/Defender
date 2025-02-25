#pragma once
#include "Entity.h"
class Astronaut :
    public Entity
{
public:
	Astronaut(sf::Vector2f pos_,
		bool         isScripted_ = false, EntityScript* script_ = nullptr)
		: Entity(pos_, EntityID::PLAYER, isScripted_, script_) {}
};

