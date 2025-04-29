#pragma once
#include "Entity.h"
class Astronaut :
    public Entity
{
public:
	explicit Astronaut(sf::Vector2f pos_,
		bool         isScripted_ = false, EntityScript* script_ = nullptr)
		: Entity(pos_, EntityID::ASTRONAUT, isScripted_, script_) {}
};

