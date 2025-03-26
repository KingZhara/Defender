#pragma once
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

	COMN() = delete;
};
