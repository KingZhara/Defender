#pragma once
#include <cstdint>

namespace EntityID
{
	enum EntityID : uint8_t
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

		LENGTH
	};
}