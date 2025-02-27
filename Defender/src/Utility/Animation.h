
#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Timer.h"

class Animation : public sf::Drawable
{
private:
	// The frame being displayed
	sf::Sprite frame;

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

	/**
	 * The sprite sheet used for all sprites
	 */
	static const sf::Texture* tex;

	/**
	 * Draw method provided by drawable
	 */
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		states.shader = shader;
		target.draw(frame, states);
	}

public:
	Animation() = delete;
	
	/**
	 * The animation constructor.
	 *
	 * @todo update these docs
	 * @param framelength The length of time (in seconds) of each frame
	 * @param length_ The length of the sprite
	 * @param bounds  The texture bounds of the sprite
	 */
	Animation(uint8_t length_,               sf::IntRect bounds,
			  double framelength = 1. / 15., sf::Shader* shader_ = nullptr)
		: frame(sf::Sprite(*tex, bounds)),
		  shader(shader_), frameTimer(Timer<double>(framelength)),
		  LENGTH(length_), start(frame.getTextureRect().left) {}

	/**
	 * 
	 * @param texture 
	 */
	static void setTexture(const sf::Texture* texture)
	{
		tex = texture;
	}

	/**
	 * 
	 * @param deltatime 
	 */
	void tick(double deltatime)
	{
		sf::IntRect newBounds = frame.getTextureRect();
		if (frameTimer.tick(deltatime))
		{
			if (++currentFrame == LENGTH)
				currentFrame = 0;

			newBounds.left = start + newBounds.width * currentFrame;
			frame.setTextureRect(newBounds);
		}
	}

	void setPosition(sf::Vector2f pos)
	{
		frame.setPosition(round(pos.x), round(pos.y));
	}

	// Used by the player for direction switching :D
	sf::Sprite& getSprite()
	{
		return frame;
	}

	sf::FloatRect getBounds() { return frame.getGlobalBounds(); }
};

