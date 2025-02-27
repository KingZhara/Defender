#include "../Player.h"

void Player::setActions(Action& actions)
{
	this->actions = actions;
	if (actions.flags.left)
	{
		left = true;
		animation.setDirection(false);
	}
	else if (actions.flags.right)
	{
		left = false;
		animation.setDirection(true);
	}
}
