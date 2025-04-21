
#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "DisplayManager.h"
#include "EntityID.h"
#include "ShaderID.h"
#include "Timer.h"

#include "UserInterface/UserInterface.h"


struct SpriteData
{
	sf::IntRect        bounds;
	uint8_t            frameCount;
	ShaderID::ID shader = ShaderID::NONE;
	double             frameLength = 1. / 2.;

	SpriteData(sf::IntRect bounds_, uint8_t frameCount_,
		ShaderID::ID shader_ = ShaderID::NONE,
		double frameLength_ = 1. / 2.) :
		bounds(bounds_), frameCount(frameCount_), shader(shader_),
		frameLength(frameLength_)
	{
	}
};

class Animation : public sf::Drawable
{
public:
	Animation(const SpriteData& SPRITE_DATA_, sf::Texture* customTex = tex);

	void tick(double deltatime);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	static void setTexture(sf::Texture* tex_);
	void setTexturePos(sf::Vector2i pos);
	void setPosition(sf::Vector2f pos);
	bool intersects(Animation& other);

private:
	static sf::Texture* tex;

	const SpriteData& SPRITE_DATA;
	sf::Texture* texture = nullptr;
	sf::Sprite frame;
	uint8_t frameIndex;
	Timer<double> fTimer;
};

inline bool Animation::intersects(Animation &other)
{
    return frame.getGlobalBounds().intersects(other.frame.getGlobalBounds());
}

/*class Animation : public sf::Drawable
{
private:
	// @todo consider moving to heap; Entities are relatively massive, these are not always necessary
	// The frame being displayed
	mutable sf::Sprite frame;

	// The shader that is applied to this animation
	sf::Shader* shader = nullptr;

	// Timing for frames
	mutable Timer<double> frameTimer;

	// The number of frames
	const uint8_t LENGTH;

	// The current frame of the animation
	uint8_t currentFrame = 0;

	// The starting x position
	uint16_t start;

	EntityID::ID ID;

	/**
	 * The sprite sheet used for all sprites
	 *//*
	static const sf::Texture* tex;

	/**
	 * Draw method provided by drawable
	 *//*
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	Animation() = delete;
	
	/**
	 * The animation constructor.
	 *
	 * @todo update these docs
	 * @param framelength The length of time (in seconds) of each frame
	 * @param length_ The length of the sprite
	 * @param bounds  The texture bounds of the sprite
	 *//*
	Animation(uint8_t length_,               sf::IntRect bounds, EntityID::ID id_,
			  double framelength = 1. / 15., sf::Shader* shader_ = nullptr)
		: frame(sf::Sprite(*tex, bounds)),
		  shader(shader_), frameTimer(Timer<double>(framelength)),
		  LENGTH(length_), start(frame.getTextureRect().left), ID(id_)
	{
		std::cout << "BOUNDS: " << bounds.getPosition().x << ", " << bounds.getPosition().y << ", " << bounds.getSize().x << ", " << bounds.getSize().y << '\n';
	}

	/**
	 *
	 * @param texture
	 *//*
	static void setTexture(const sf::Texture* texture) { tex = texture; }

	/**
	 *
	 * @param texture
	 *//*
	void setShader(sf::Shader* shader_) { shader = shader_; }

	/**
	 * 
	 * @param deltatime 
	 *//*
	void tick(double deltatime);

	void setStart(uint16_t start_) { start = start_; }

	void setPosition(sf::Vector2f pos) const;

	// Used by the player for direction switching :D
	sf::Sprite& getSprite() const
	{
		return frame;
	}

	sf::FloatRect getBounds() const { return frame.getGlobalBounds(); }

	void setTexturePos(sf::Vector2i pos)
	{
		sf::IntRect rect = frame.getTextureRect();
		rect.left = pos.x;
		rect.top = pos.y;
		frame.setTextureRect(rect);
		start = pos.x;
	}
};

*/