#pragma once
#include <cstdint>

#include <SFML/Graphics/Glsl.hpp>

#include "SFML/System/Vector2.hpp"

template <typename T>
struct Vec2
{
	T x, y;

	constexpr Vec2(T x_, T y_) : x(x_), y(y_) {}
};

template <typename T>
struct Color
{
	T r, g, b;

	constexpr Color(T r_, T g_, T b_) : r(r_), g(g_), b(b_) {}

	operator sf::Color() const
	{
		return sf::Color((uint8_t)r, (uint8_t)g, (uint8_t)b);
	}

	operator sf::Glsl::Vec3() const
	{
		return sf::Glsl::Vec3((float)(r / 255.0f), (float)(g / 255.0f), (float)(b / 255.0f));
	}
};

// @deprecated Move on to converted usages
struct COMN
{
	static constexpr Vec2<float> resolution
	{
		292,
		240
	};
	static constexpr float uiHeight = 34;
	static constexpr Vec2<float> aspectRatio
	{
		73,
		60
	};

	static constexpr Vec2<double> baseSpeed
	{
		150.f,
		100.f
	};

	struct playHeight
	{
		static constexpr uint8_t min = uiHeight;
		static constexpr uint8_t max = resolution.y;
	};

	static constexpr float playerSpeedFactor = 1.5;

	static constexpr uint16_t worldSize = static_cast<uint16_t>(9 * resolution.x);

	static constexpr uint8_t worldBgHeight = 80;
	static constexpr uint8_t world_startY = 20;
	static constexpr Color<double> ShaderTarget{ 136, 0, 255 };



	COMN() = delete;
};
