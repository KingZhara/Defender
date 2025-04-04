#include "../Animation.h"


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
