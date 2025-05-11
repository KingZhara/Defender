#pragma once
#include <cstdint>
#include "../EntityManager.h"
#include "EntityHolder.h"
#include <unordered_map>


class EntityManager::LanderTargetTable
{
	std::unordered_map<uint16_t, uint16_t> landerToAstronaut;
	std::unordered_map<uint16_t, uint16_t> astronautToLander;
	bool                                   hasplayerReserved;
	uint16_t                               playerReservedIndex;

public:
	LanderTargetTable() = default;

	bool isLinkedAstro(uint16_t index)
	{
		return astronautToLander.count(index);
	}

	bool isLinkedLander(uint16_t index)
	{
		return landerToAstronaut.count(index);
	}

	void link(uint16_t lander, uint16_t astronaut)
	{
		landerToAstronaut[lander] = astronaut;
		astronautToLander[astronaut] = lander;
	}

	void unlinkAstro(uint16_t astronaut)
	{
		landerToAstronaut.erase(astronautToLander[astronaut]);
		astronautToLander.erase(astronaut);
	}

	void unlinkLander(uint16_t lander)
	{
		astronautToLander.erase(landerToAstronaut[lander]);
		landerToAstronaut.erase(lander);
	}

	Lander* getLander(uint16_t astroIndex)
	{
		return dynamic_cast<Lander*>(enemies.entities.at(astronautToLander[astroIndex]));
	}

	Astronaut* getAstro(uint16_t landerIndex)
	{
		return dynamic_cast<Astronaut*>(astronauts.entities.at(landerToAstronaut[landerIndex]));
	}

	uint16_t getLanderIndex(uint16_t astroIndex)
	{
		return astronautToLander[astroIndex];
	}

	uint16_t getAstroIndex(uint16_t landerIndex)
	{
		return landerToAstronaut[landerIndex];
	}

	void reset()
	{
		landerToAstronaut.clear();
		astronautToLander.clear();
	}
};
