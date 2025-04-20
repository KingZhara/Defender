#include "../Animation.h"

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
Animation::Animation(SpriteData &SPRITE_DATA_, sf::Texture* customTex)
	: SPRITE_DATA(SPRITE_DATA_),
	frame(*customTex, SPRITE_DATA_.bounds),
	fTimer(SPRITE_DATA_.frameLength),
	texture(customTex)
{
    
}

void Animation::tick(double deltatime)
{
	// If next frame
	// move frame based on current frame index + 1

	if (fTimer.tick(deltatime))
	{
		frameIndex = (frameIndex + 1) % SPRITE_DATA.frameCount;
		sf::IntRect newBounds = frame.getTextureRect();
		newBounds.left += newBounds.width * (frameIndex + 1) + frameIndex;
		frame.setTextureRect(newBounds);
	}

}

void Animation::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.shader = UserInterface::getShader(SPRITE_DATA.shader);
	target.draw(frame, states);
}
