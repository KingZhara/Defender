#pragma once

struct Action
{
	struct Flags
	{
		uint8_t left       : 1 = 0;
		uint8_t right      : 1 = 0;
		uint8_t thrust     : 1 = 0;
		uint8_t fire       : 1 = 0;
		uint8_t smart_bomb : 1 = 0;
		uint8_t hyperspace : 1 = 0;
		uint8_t up		   : 1 = 0;
		uint8_t down	   : 1 = 0;

		operator bool() const noexcept
		{
			return left || right || thrust || fire || smart_bomb || hyperspace || up || down;
		}
	};

	Flags flags;

	operator bool() const noexcept { return flags; }

	// Do not mark explicit
	Action(Flags flags_) : flags(flags_) {}

	Action() = default;
};

