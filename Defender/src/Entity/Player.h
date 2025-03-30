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
	}


	virtual void tick(double deltatime) override
    {
		Entity::tick(deltatime);
		vel *= 0.9f;

		if (actions.flags.thrust)
			vel.x += (actions.flags.left ? -0.25f : +0.25f);
		if (actions.flags.up)
			vel.y -= 0.2f;
		if (actions.flags.down)
			vel.y += 0.2f;
		if (actions.flags.fire)
			entityQueue.emplace(pos, EntityID::LASER);

        pos.y = std::max<float>(pos.y, 0);

        pos.y = std::min(pos.y, COMN::resolution.y - getBounds(EntityID::PLAYER).height);

		//pos.x = static_cast<float>(
		//	static_cast<int>(std::round(pos.x + getBounds(EntityID::PLAYER).width / 2)) %
		//	static_cast<int>(std::round(DisplayManager::getView().getSize().x * 9)));
    }

    void setActions(Action& actions);


private:
    Action actions;

};
