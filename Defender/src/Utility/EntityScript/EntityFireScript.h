#pragma once
#include "EntityScript.h"
#include "../Timer.h"

struct EntityFireScript : EntityScript
{
	EntityFireScript(EntityScript* next_ = nullptr)
		: EntityScript(EntityScript::ScriptType::WAIT, next_)
	{
	}

	virtual bool tick(sf::Vector2f pos = {}, double deltatime = 0.)
	{
		
	}
};

