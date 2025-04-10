#include "../Player.h"

void Player::setActions(Action& actions)
{
	this->actions = actions;
	animation.setTexturePos(sf::Vector2i(
		DATA_TABLE[EntityID::PLAYER].SPRITE_DATA.bounds.width * 2 * actions.flags.left, 16));
}
