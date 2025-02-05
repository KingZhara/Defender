#pragma once

#include <SFML/Graphics.hpp>

#include "Timer.h"

class Animation : sf::Drawable
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

	// Space between frames
	uint8_t margin;

	// The current frame of the animation
	uint8_t currentFrame = 0;

	// The starting x position
	uint16_t start;

	/**
	 * The sprite sheet used for all sprites
	 */
	static sf::Texture tex;

	/**
	 * Draw method provided by drawable
	 *
	 * @todo allow for shaders w/ randerstates
	 */
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		states.shader = shader;
		target.draw(frame);
	}

public:
	Animation() = delete;

	/**
	 * The animation constructor.
	 * 
	 * @param framelength The length of time (in seconds) of each frame
	 * @param margin_ The space between individual sprites
	 * @param length_ The length of the sprite
	 * @param bounds  The texture bounds of the sprite
	 */
	Animation(double framelength, uint8_t margin_, uint8_t length_, sf::IntRect bounds, sf::Shader* shader_ = nullptr)
		: frameTimer(Timer<double>(framelength)), margin(margin_),
		  frame(sf::Sprite(tex, bounds)), LENGTH(length_), shader(shader_), start(frame.getTextureRect().left) {}

	/**
	 * 
	 * @param texture 
	 */
	static void setTexture(const sf::Texture& texture)
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

			newBounds.left = start + (margin + newBounds.width) * currentFrame;
			frame.setTextureRect(newBounds);
		}
	}
};

