#pragma once
#include "EntityScript.h"
#include "../Timer.h"

struct EntityWaitScript : EntityScript
{
	Timer<double> target;

	EntityWaitScript(double target_ = 0, EntityScript * next_ = nullptr)
		: EntityScript(EntityScript::ScriptType::WAIT, next_), target(target_) {}

	virtual bool tick(sf::Vector2f pos = {}, double deltatime = 0.)
	{
		return target.tick(deltatime);
	}
};

