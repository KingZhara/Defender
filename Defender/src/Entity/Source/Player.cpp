#include "../Player.h"

void Player::setActions(Action& actions)
{
	sf::IntRect texRect = animation.getSprite().getTextureRect();

	this->actions = actions;
	if (actions.flags.left)
	{
		left = true;

		texRect.left = 0;
	}
	else if (actions.flags.right)
	{
		left = false;

		texRect.left = SPRITE_TABLE[EntityID::PLAYER].bounds.left * 2;
	}


	animation.getSprite().setTextureRect(texRect);
}
