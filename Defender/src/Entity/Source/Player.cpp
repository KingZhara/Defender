#include "../Player.h"

void Player::setActions(Action& actions)
{
	this->actions = actions;
	if (actions.flags.left)
	{
		left = true;
		animation.setTexturePos(sf::Vector2i(SPRITE_TABLE[EntityID::PLAYER].bounds.width * 2, 16));
	}
	if (actions.flags.right)
	{
		left = false;
		animation.setTexturePos(sf::Vector2i(0, 16));
	}
}
