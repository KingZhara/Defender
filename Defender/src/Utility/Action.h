#pragma once

struct Action
{
	// NULL, NULL, left, right, thrust, fire, smart_bomb, hyperspace
	uint8_t flags = 0;

	operator bool() const noexcept { return flags; }
};

