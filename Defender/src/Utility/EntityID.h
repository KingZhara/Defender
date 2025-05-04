#pragma once
#include <cstdint>

struct EntityID
{
	enum ID : uint8_t
	{
		PLAYER,
		ASTRONAUT,

		BULLET,
		LASER,
		BOMB,

		LANDER,
		MUTANT,
		BAITER,
		BOMBER,
		POD,
		SWARMER,

		PARTICLE,
		PIECE,
	    DEATH_ANIM_PIECE,

		LENGTH
	};
};