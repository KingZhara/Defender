#pragma once
#include <SFML/Graphics.hpp>
#include "../Timer.h"
#include "../EntityID.h"
#include "../ShaderID.h"


struct SpriteTableEntry
{
	sf::IntRect        bounds;
	uint8_t            frameCount, frameIndex;
	ShaderID::ID shader;
	Timer<double> frameTimer;
	sf::Sprite* sprite;

	SpriteTableEntry(sf::IntRect bounds_, uint8_t frameCount_,
		ShaderID::ID shader_ = ShaderID::NONE,
		double frameLength_ = 1. / 2.) :
		bounds(bounds_), frameCount(frameCount_), shader(shader_),
		frameTimer(frameLength_), sprite(nullptr), frameIndex(0)
	{
	}
};

class VisualComponent : public sf::Drawable
{
public:
	VisualComponent(EntityID::ID ID);

	void setPosition(sf::Vector2f pos_);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual bool intersects(sf::FloatRect other);
	bool intersects(VisualComponent* other);

	static void tick(double deltatime);
	static void initialize(sf::Texture* tex_);
	static SpriteTableEntry& getPlayerData();
	static sf::IntRect getBounds(EntityID::ID ID);
protected:
	void patchDrawData(sf::Sprite*, sf::RenderStates& states) const;

private:
	static SpriteTableEntry DATA_TABLE[EntityID::LENGTH];
	static sf::Texture* tex;

	sf::Vector2f pos;
	EntityID::ID ID;
};

