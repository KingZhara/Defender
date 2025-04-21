#include "../Animation.h"

sf::Texture* Animation::tex = nullptr;

/*
const sf::Texture*Animation::tex = nullptr;

void Animation::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	UserInterface::getFlashingShader()->setUniform("textureSize", sf::Glsl::Vec2{ frame.getGlobalBounds().getSize() });

	if (shader)
		shader->setUniform("texture", sf::Shader::CurrentTexture);

	states.shader = shader;
	target.draw(frame, states);
}

void              Animation::tick(double deltatime)
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

void Animation::setPosition(sf::Vector2f pos) const
{
	frame.setPosition(round(pos.x), round(pos.y));
}
*/
Animation::Animation(const SpriteData &SPRITE_DATA_, sf::Texture* customTex)
	: SPRITE_DATA(SPRITE_DATA_),
	frame(*customTex, SPRITE_DATA_.bounds),
	fTimer(SPRITE_DATA_.frameLength),
	texture(customTex)
{
	std::cout << "ANIM MAKE BOUNDS: " << SPRITE_DATA.bounds.left << ", " << SPRITE_DATA.bounds.top << ", " << SPRITE_DATA.bounds.width << ", " << SPRITE_DATA.bounds.height << '\n';
}

void Animation::tick(double deltatime)
{
	// If next frame
	// move frame based on current frame index + 1

	if (SPRITE_DATA.frameCount > 1 && fTimer.tick(deltatime))
	{
		// Everything is relative to current bounds because
		// the player needs to change textures
		sf::IntRect newBounds = frame.getTextureRect();
		if (++frameIndex == SPRITE_DATA.frameCount)
		{
			frameIndex = 0;
			std::cout << "Old Left: " << newBounds.left << " New Left: ";
			newBounds.left -= newBounds.width * (SPRITE_DATA.frameCount - 1) + SPRITE_DATA.frameCount - 1;
			std::cout << newBounds.left << '\n';
		}
		else
		{
			newBounds.left += newBounds.width + 1;
			std::cout << "ANIM BOUNDS: " << newBounds.left << ", " << newBounds.top << ", " << newBounds.width << ", " << newBounds.height << '\n';
		}

		frame.setTextureRect(newBounds);
	}

}

void Animation::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.shader = UserInterface::getShader(SPRITE_DATA.shader);
	target.draw(frame, states);
}

void Animation::setTexture(sf::Texture *tex_)
{
	tex = tex_;
}

void Animation::setTexturePos(sf::Vector2i pos)
{
	frame.setTextureRect(sf::IntRect(pos.x, pos.y, SPRITE_DATA.bounds.width, SPRITE_DATA.bounds.height));
}

void Animation::setPosition(sf::Vector2f pos)
{
	frame.setPosition(round(pos.x), round(pos.y));
}
