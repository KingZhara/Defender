#pragma once
#include <cstdint>

struct ShaderID
{
	enum ID : uint8_t
	{
		NONE,
		HUE_SHIFT,
		STARS,
		RAND_COL
	};
};