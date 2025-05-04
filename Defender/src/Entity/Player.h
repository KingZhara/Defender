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

	~Player()
	{
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

		//std::cout << "PLAYER FLAGS: F: " << (short)actions->flags.fire << '\n';

		if (actions->flags.fire)
		{
			entityQueue.emplace(sf::Vector2f{pos.x - 4, pos.y}, EntityID::LASER);
			actions->flags.fire = false; // In theory this is redundant, yet here we are.
		}
		Entity::tick(deltatime);

    }

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override
	{
		miniSprite->setTextureRect(sf::IntRect(80, 3, 3, 3));

		Entity::draw(target, states);
	}

    static void setActions(Action& actions);

	bool collide(Entity *other) override
	{
		return false; // Entity::collide(other);
	}

	bool getDir() const
	{
		return left;
	}

private:
	void processActions()
	{
		if (actions->flags.left)
		{
			actions->flags.left = false;
			left = !left;

			uint8_t index = animation.getFrameIndex() + (left ? 2 : 0);

			animation.setTexturePos({
				DATA_TABLE[EntityID::PLAYER].SPRITE_DATA.bounds.left + DATA_TABLE[EntityID::PLAYER].SPRITE_DATA.bounds.width * index + index,
				DATA_TABLE[EntityID::PLAYER].SPRITE_DATA.bounds.top
				});
		}
	}
	virtual void wrap() override
	{
		float diff = pos.x - DisplayManager::getView().getCenter().x;
		float x = pos.x;

		Entity::wrap();

		if (x != pos.x)
    		DisplayManager::getView().setCenter(
	    		pos.x - diff,
		    	DisplayManager::getView().getCenter().y);

		pos.y = std::max<float>(pos.y, COMN::uiHeight);

		pos.y = std::min(pos.y, COMN::resolution.y - getBounds(EntityID::PLAYER).height);

		//std::cout << "PLAYER POS: " << pos.x << '\n';
	}

    static Action* actions;
	bool left = false;
};
