#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "../Utility/ShaderID.h"
#include "../Utility/EntityID.h"
#include "../Utility/Animation.h"
#include "../Utility/EntityScript/EntityScript.h"

// new EntityScript(EntityScript::ScriptType::DONE) - BLANK SCRIPT

class Entity : public sf::Drawable
{
public:
	// @todo Complete shader retrieval through UI when possible
	Entity(sf::Vector2f pos_,                EntityID::EntityID ID_,
		   bool         isScripted_ = false, EntityScript*      script_ = nullptr)
		: pos(pos_), ID(ID_), isScripted(isScripted_), script(script_),
		  animation(SPRITE_TABLE[ID].frameCount, SPRITE_TABLE[ID].bounds, SPRITE_TABLE[ID].frameLength/*SPRITE_TABLE[ID].shader*/) {}

	void tick();

	bool collide(Entity* other);

	void setPos(sf::Vector2f newPos);
	sf::Vector2f getPos() { return pos; }
	EntityID::EntityID getID() { return ID; }

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

	// The data table used for generating a given sprite
	static const SpriteData SPRITE_TABLE[EntityID::LENGTH];

	// The data table used for generating a given sprite
	static const uint16_t XP_TABLE[EntityID::LENGTH];

	sf::Vector2f pos, vel;

	bool isScripted;

	EntityScript* script;

	Animation animation;

	const EntityID::EntityID ID;
};

 