#include "../Player.h"

void Player::setActions(Action& actions)
{
	this->actions = actions;
	animation.setTexturePos(sf::Vector2i(
		SPRITE_TABLE[EntityID::PLAYER].bounds.width * 2 * actions.flags.left, 16));
}
