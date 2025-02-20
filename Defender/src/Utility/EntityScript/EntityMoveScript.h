#pragma once
#include <SFML/System/Vector2.hpp>
#include "EntityScript.h"

struct EntityMoveScript : EntityScript
{
	sf::Vector2f target;

	EntityMoveScript(ScriptType type_, sf::Vector2f target_ = {}, EntityScript* next_ = nullptr)
		: EntityScript(type_, next_), target(target_) {}

	virtual bool tick(sf::Vector2f pos = {}, double deltatime = 0.)
	{
		// @todo Complete move script logic IE direction vector generation & checking against given pos w/ deltatime value
		return true;
	}
};

