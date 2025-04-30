#pragma once
#include "Entity.h"
class Astronaut :
    public Entity
{
public:
	explicit Astronaut(sf::Vector2f pos_,
		bool         isScripted_ = false, EntityScript* script_ = nullptr)
		: Entity(pos_, EntityID::ASTRONAUT, isScripted_, script_) {}

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

