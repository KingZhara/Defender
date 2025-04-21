#pragma once
#include <SFML/System/Vector2.hpp>
#include "EntityScript.h"

struct EntitySpawnScript : EntityScript
{
	sf::Vector2f pos;
	int entityID, scriptID;

	EntitySpawnScript(int entityID_ = 0, sf::Vector2f pos_ = {}, int scriptID_ = 0, EntityScript* next_ = nullptr)
		: EntityScript(EntityScript::ScriptType::SPAWN, next_), pos(pos_), entityID(entityID_), scriptID(scriptID_)
	{
	}

	virtual bool tick(sf::Vector2f& pos, double deltatime = 0.)
	{
		// @todo Complete move script logic IE direction vector generation & checking against given pos w/ deltatime value
		return true;
	}
};

