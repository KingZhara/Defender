#pragma once
#include <SFML/System/Vector2.hpp>
#include "EntityScript.h"

struct EntityMoveScript : EntityScript
{
	sf::Vector2f target;

	EntityMoveScript(sf::Vector2f target_ = {}, EntityScript* next_ = nullptr)
		: EntityScript(EntityScript::ScriptType::MOVE, next_), target(target_) {}

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

