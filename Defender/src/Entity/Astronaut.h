#pragma once
#include "Entity.h"
class Astronaut :
    public Entity
{
public:
	explicit Astronaut(sf::Vector2f pos_,
		bool         isScripted_ = false, EntityScript* script_ = nullptr)
		: Entity(pos_, EntityID::ASTRONAUT, isScripted_, script_), onGround(true), held(false) {}


	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	virtual void tick(double deltatime) override;
	void setTargeted(bool v);
	bool targeted();
	bool isOnGround()   const { return onGround; }
	void setHolder(Entity* holder_);

private:
	bool isTargeted;
	bool held;
	bool onGround;

	// Player and lander can set holder
	/**
	 * Add fall length; kill if > COMN::Resolution.y / 4
	 *
	 */
};

