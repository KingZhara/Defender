#pragma once
#include "Entity.h"
#include <numbers>
#include "../Utility/Action.h"

class Player :
    public Entity
{
public:
	Player(sf::Vector2f pos_,
		bool         isScripted_ = false, EntityScript* script_ = nullptr)
		: Entity(pos_, EntityID::PLAYER, isScripted_, script_)
	{
		playerPos = &pos;
	}


	virtual void tick(double deltatime) override
    {
		Entity::tick(deltatime);

		if (actions.flags.thrust)
			vel.x += (actions.flags.left ? -0.25f : +0.25f);
		if (actions.flags.up)
			vel.y -= 0.2f;
		if (actions.flags.down)
			vel.y += 0.2f;
		if (actions.flags.fire)
			Entity::entityQueue.emplace(pos, vel.x > 0 ? 0 : std::numbers::pi, EntityID::LASER);
	}

    void setActions(Action& actions);


private:
    Action actions;

};
