#pragma once
#include <unordered_map>
#include <SFML/Graphics/Rect.hpp>
#include "../Utility/ShaderID.h"
#include "../Utility/EntityID.h"

class Entity
{
public:
private:
	struct SpriteData
	{
		sf::IntRect bounds;
		uint8_t frameCount;
		ShaderID::ShaderID shader = ShaderID::NONE;
		double frameLength = 1./15.;
	};

	static const SpriteData SpriteTable[EntityID::LENGTH];
};

