#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "../Utility/ShaderID.h"
#include "../Utility/EntityID.h"

class Entity : public sf::Drawable
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

	// The data table used for generating a given sprite
	static const SpriteData SPRITE_TABLE[EntityID::LENGTH];

	// The data table used for generating a given sprite
	static const uint16_t XP_TABLE[EntityID::LENGTH];

	virtual bool tick();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

 