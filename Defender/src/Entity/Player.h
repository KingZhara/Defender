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

		if (actions.flags.thrust)
			vel.x += (left ? -0.5f : +0.5f);
		if (actions.flags.up)
			vel.y -= 0.2f;
		if (actions.flags.down)
			vel.y += 0.2f;
	}

    void setActions(Action& actions);
private:
    Action actions;
	bool left = false; // false: right, true: left

};
