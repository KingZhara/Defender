#pragma once

struct Action
{
	// NULL, NULL, left, right, thrust, fire, smart_bomb, hyperspace
	uint8_t flags = 0;

	operator bool() const noexcept { return flags; }

	// Do not mark explicit
	Action(uint8_t flags_) : flags(flags_) {}

	Action() = default;
};

