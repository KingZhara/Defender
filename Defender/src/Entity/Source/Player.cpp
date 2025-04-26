#include "../Player.h"


Action* Player::actions = nullptr;

void Player::setActions(Action& actions_)
{
	actions = &actions_;
}
