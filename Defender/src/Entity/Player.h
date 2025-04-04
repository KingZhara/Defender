#pragma once
#include <algorithm>
#include <numbers>
#include "Entity.h"
#include "../Utility/Action.h"
#include "../Utility/DisplayManager.h"
#include "../Utility/common.h"

class Player :
    public Entity
{
public:
	Player(sf::Vector2f pos_,
		bool         isScripted_ = false, EntityScript* script_ = nullptr)
		: Entity(pos_, EntityID::PLAYER, isScripted_, script_)
	{
		playerPos = &pos;
		playerVel = &vel;
	}


	virtual void tick(double deltatime) override
    {

		if (actions.flags.thrust)
			vel.x = (actions.flags.left ? -1 :1) * COMN::baseSpeed.x * COMN::playerSpeedFactor;
		else
			vel.x *= 0.9 * deltatime;
		if (actions.flags.up)
			vel.y = -COMN::baseSpeed.y * COMN::playerSpeedFactor;
		if (actions.flags.down)
			vel.y = COMN::baseSpeed.y * COMN::playerSpeedFactor;
		if (actions.flags.fire)
			entityQueue.emplace(pos, EntityID::LASER);

        pos.y = std::max<float>(pos.y, 0);

        pos.y = std::min(pos.y, COMN::resolution.y - getBounds(EntityID::PLAYER).height);

		Entity::tick(deltatime);
    }

    void setActions(Action& actions);

	bool collide(Entity *other) override
	{
		std::cout << "PCOL\n" << '\n';
	    return Entity::collide(other);
	}

	bool getDir() const
	{
		return actions.flags.left;
	}

private:
    Action actions;
};
