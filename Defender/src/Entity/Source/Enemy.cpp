#include "../Enemy.h"

Enemy::Enemy(sf::Vector2f pos_,
        EntityID::ID id_,
        bool isScripted_,
        EntityScript *script_): Entity(pos_, id_, isScripted_, script_) {}

void Enemy::tick(double deltatime)
{
    Entity::tick(deltatime);

	if (pos.y > COMN::playHeight::max)
		pos.y -= COMN::playHeight::max - COMN::playHeight::min;
	else if (pos.y < COMN::playHeight::min)
		pos.y += COMN::playHeight::max - COMN::playHeight::min;
}

void Enemy::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    miniSprite->setTextureRect(sf::IntRect(80 + (ID - EntityID::LANDER) * 3, 0, 2, 2));
    Entity::draw(target, states);
}
