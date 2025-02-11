#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "../Utility/ShaderID.h"
#include "../Utility/EntityID.h"
#include "../Utility/Animation.h"

class Entity
{
public:
	bool tick();

	bool collide(Entity* other);

	void setPos(sf::Vector2f newPos);

private:

	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(animation, states);
	}
	
	struct SpriteData
	{
		sf::IntRect bounds;
		uint8_t frameCount;
		ShaderID::ShaderID shader = ShaderID::NONE;
		double frameLength = 1./15.;
	};

	static const SpriteData SpriteTable[EntityID::LENGTH];

	sf::Vector2f pos, vel;

	bool isScripted;
	// TODO
	//EntityScript* script;

	Animation animation;
};

 