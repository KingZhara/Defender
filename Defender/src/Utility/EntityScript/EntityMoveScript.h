#pragma once
#include <SFML/System/Vector2.hpp>
#include "EntityScript.h"

struct EntityMoveScript : EntityScript
{
	sf::Vector2f target;
	const float INIT_X;

	EntityMoveScript(ScriptType type_, sf::Vector2f target_ = {}, float INIT_X_, EntityScript* next_ = nullptr)
		: EntityScript(type_, next_), target(target_), INIT_X(INIT_X_) {}

	virtual bool tick(sf::Vector2f& pos, double deltatime = 0.)
	{
		double rot = atan2(
			target.y - pos.y,
			target.x - pos.x
		);
		// @todo Complete move script logic IE direction vector generation & checking against given pos w/ deltatime value
		return true;
	}
};

