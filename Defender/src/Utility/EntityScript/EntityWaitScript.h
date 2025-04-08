#pragma once
#include "EntityScript.h"
#include "../Timer.h"

struct EntityWaitScript : EntityScript
{
	Timer<double> target;

	EntityWaitScript(ScriptType type_, double target_ = 0, EntityScript * next_ = nullptr)
		: EntityScript(type_, next_), target(target_) {}

	virtual bool tick(sf::Vector2f pos = {}, double deltatime = 0.)
	{
		return target.tick(deltatime);
	}
};

