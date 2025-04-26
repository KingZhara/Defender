#pragma once
#include <algorithm>
#include <numbers>
#include "Entity.h"
#include "../Utility/Action.h"
#include "../Utility/DisplayManager.h"
#include "../Utility/common.h"

//@todo fix player sprite

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
		processActions();

		// Horizontal Movement
		if (actions->flags.thrust)
			vel.x = (float)((left ? -1 :1) * COMN::baseSpeed.x * COMN::playerSpeedFactor);
		else
			vel.x *= (float)(0.9 * deltatime);
		if (actions->flags.up)
			vel.y = (float)(-COMN::baseSpeed.y * COMN::playerSpeedFactor);
		else if (actions->flags.down)
			vel.y = (float)(COMN::baseSpeed.y * COMN::playerSpeedFactor);
		else
			vel.y *= (float)(0.9 * deltatime);

		std::cout << "PLAYER FLAGS: F: " << (short)actions->flags.fire << '\n';

		if (actions->flags.fire)
		{
			entityQueue.emplace(pos, EntityID::LASER);
			actions->flags.fire = false; // In theory this is redundant, yet here we are.
		}

        pos.y = std::max<float>(pos.y, COMN::uiHeight);

        pos.y = std::min(pos.y, COMN::resolution.y - getBounds(EntityID::PLAYER).height);

		Entity::tick(deltatime);
    }

    static void setActions(Action& actions);

	bool collide(Entity *other) override
	{
	    return Entity::collide(other);
	}

	bool getDir() const
	{
		return left;
	}

private:
	void processActions()
	{
		static bool wait = false;

		//animation.setTexturePos(sf::Vector2i(
		//	DATA_TABLE[EntityID::PLAYER].SPRITE_DATA.bounds.width * 2 * actions.flags.left, 16));

		if (actions->flags.left)
		{
			actions->flags.left = false;
			left = !left;

			uint8_t index = animation.getFrameIndex() + (left ? 2 : 0);
			wait = true;

			animation.setTexturePos({
				DATA_TABLE[EntityID::PLAYER].SPRITE_DATA.bounds.left + DATA_TABLE[EntityID::PLAYER].SPRITE_DATA.bounds.width * index + index,
				DATA_TABLE[EntityID::PLAYER].SPRITE_DATA.bounds.top
				});
		}
	}
	virtual void wrap() override
	{
		double diff = pos.x - DisplayManager::getView().getCenter().x;
		uint16_t x = (uint16_t)pos.x;

		Entity::wrap();

		if (x != (uint16_t)pos.x)
    		DisplayManager::getView().setCenter(
	    		DisplayManager::getView().getCenter().x + (pos.x - diff),
		    	DisplayManager::getView().getCenter().y);

		//std::cout << "PLAYER POS: " << pos.x << '\n';
	}

    static Action* actions;
	bool left = false;
};
