#pragma once
#include <cstdint>
#include "../EntityManager.h"
#include "EntityHolder.h"
#include <unordered_map>


class EntityManager::LanderTargetTable
{

	uint16_t lander = 0, astro = 0, playerAstro = 0;
	bool playerLinked = false, landerLinked = false;

public:
	LanderTargetTable() = default;

	void reserve(uint16_t astroIndex)
	{
		playerLinked = true;
		playerAstro = astroIndex;
		if (landerLinked && astro == astroIndex)
		{
			getLander()->setTarget(nullptr);

		    unlinkLander();
		}
		dynamic_cast<Astronaut*>(astronauts.entities.at(astroIndex))->setTargeted(true);

	}

	bool isLanderLinked()
	{
		return landerLinked;
	}

	void linkLander(uint16_t lander_, uint16_t astronaut)
	{
		lander = lander_;
		astro = astronaut;

		landerLinked = true;

		getLander()->setTarget(getAstro(false));
		getAstro(false)->setTargeted(true);
		getAstro(false)->setHolder(getLander());
	}

	void linkPlayer(uint16_t astroIndex)
	{
		playerLinked = true;
		playerAstro = astroIndex;
		if (landerLinked && astro == astroIndex)
		{
			getLander()->setTarget(nullptr);
			unlinkLander();
		}
		getAstro(true)->setTargeted(true);
		getAstro(true)->setHolder(player);
		player->setAstro(getAstro(true));
	}

	void unlinkLander()
	{
		unlinkLander(lander);
	}

	void unlinkLander(uint16_t index)
	{
  		if (lander == index)
		{
			if (getLander())
				getLander()->setTarget(nullptr);

			if (getAstro(false))
			{
				getAstro(false)->setTargeted(false);
				getAstro(false)->setHolder(nullptr);
			}

			landerLinked = false;
		}
	}
	void unlinkPlayer()
	{
		if (getAstro(true))
		{
			getAstro(true)->setTargeted(false);
			getAstro(true)->setHolder(nullptr);
		}

		player->setAstro(nullptr);

		playerLinked = false;
	}
	void unlinkAstro(uint16_t index)
	{
		if (index == playerAstro)
			unlinkPlayer();
		else if (index == astro)
			unlinkLander();
	}

	Lander* getLander()
	{
	    return landerLinked ? dynamic_cast<Lander*>(enemies.entities.at(lander)) : nullptr;
	}

	Astronaut* getAstro(bool player)
	{
		bool valid = (player ? playerLinked : landerLinked);
		uint16_t index = player ? playerAstro : astro;
		return valid ? dynamic_cast<Astronaut*>(astronauts.entities.at(index)) : nullptr;
	}

	uint16_t getLanderIndex()
	{
		return lander;
	}

	uint16_t getAstroIndex(bool player)
	{
		return player ? playerAstro : astro;
	}

	void reset()
	{
		lander = playerAstro = astro = 0;
		playerLinked = landerLinked = false;
	}
};
