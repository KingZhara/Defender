#pragma once
#include "Entity.h"
#include "../Utility/Action.h"

class Player :
    public Entity
{
public:
	Player(sf::Vector2f pos_,
		bool         isScripted_ = false, EntityScript* script_ = nullptr)
		: Entity(pos_, EntityID::PLAYER, isScripted_, script_) {}


	virtual void tick(double deltatime)
	{
		Entity::tick(deltatime);
	}

    void setActions(Action& actions);
private:
    Action actions;

};
