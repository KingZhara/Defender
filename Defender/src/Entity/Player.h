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
		Entity::EntityData::PlayerRef::pos = &pos;
		Entity::EntityData::PlayerRef::vel = &vel;
	}


	virtual void tick(double deltatime) override
    {
		// Horizontal Movement
		if (actions.flags.thrust)
			vel.x = (float)((actions.flags.left ? -1 :1) * COMN::baseSpeed.x * COMN::playerSpeedFactor);
		else
			vel.x *= (float)(0.9 * deltatime);
		if (actions.flags.up)
			vel.y = (float)(-COMN::baseSpeed.y * COMN::playerSpeedFactor);
		else if (actions.flags.down)
			vel.y = (float)(COMN::baseSpeed.y * COMN::playerSpeedFactor);
		else
			vel.y *= (float)(0.9 * deltatime);

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
