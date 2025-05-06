#pragma once
#include "Entity.h"
class Astronaut :
    public Entity
{
public:
	explicit Astronaut(sf::Vector2f pos_,
		bool         isScripted_ = false, EntityScript* script_ = nullptr)
		: Entity(pos_, EntityID::ASTRONAUT, isScripted_, script_) {}

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override
	{
		miniSprite->setTextureRect(sf::IntRect(84, 3, 2, 2));

		Entity::draw(target, states);
	}

	virtual void tick(double deltatime) override
	{

	    if (pos.y < (uint16_t)COMN::resolution.y - 20)
	    {
			vel = getEVel(ID);
			vel.x = 0;
	    }

		Entity::tick(deltatime);
	}

	void setTargeted(bool v)
	{
		isTargeted = v;
	}
	bool targeted();
private:
	bool isTargeted;
	/**
	 * Add fall length; kill if > COMN::Resolution.y / 4
	 *
	 */
};

