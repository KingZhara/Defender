#pragma once

struct Action
{
	struct Flags
	{
		uint8_t left       : 1 = 0;
		uint8_t thrust     : 1 = 0;
		uint8_t fire       : 1 = 0;
		uint8_t smart_bomb : 1 = 0;
		uint8_t hyperspace : 1 = 0;
		uint8_t up		   : 1 = 0;
		uint8_t down	   : 1 = 0;
		uint8_t leftHS	   : 1 = 0; // For highscore because the other left was weird

		operator bool() const noexcept
		{
			return left || thrust || fire || smart_bomb || hyperspace || up || down;
		}
	};

	Flags flags;

	operator bool() const noexcept { return flags; }

	Action& operator=(const Action& other)
	{
		flags = other.flags;

		return *this;
	}

	// Do not mark explicit
	Action(Flags flags_) : flags(flags_) {}

	Action() = default;
};

