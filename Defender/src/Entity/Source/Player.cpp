#include "../Player.h"

void Player::setActions(Action& actions)
{
	//sf::IntRect texRect = animation.getSprite().getTextureRect();

	this->actions = actions;
	if (actions.flags.left)
	{
		left = true;

		animation.setStart(0);
	}
	else if (actions.flags.right)
	{
		left = false;

	}


	//animation.getSprite().setTextureRect(texRect);
}
