#pragma once
#include <cstdint>

#include "SFML/System/Vector2.hpp"

struct Vec2f
{
	float x, y;

	constexpr Vec2f(float x_, float y_) : x(x_), y(y_) {}
};

struct COMN
{
	static constexpr Vec2f resolution
	{
		292,
		240
	};
	static constexpr Vec2f aspectRatio
	{
		73,
		60
	};

	static constexpr uint16_t worldSize = static_cast<uint16_t>(9 * resolution.x);

	static constexpr uint8_t worldBgHeight = 80;
	static constexpr uint8_t world_startY = 20;

	COMN() = delete;
};
