#pragma once
#include "Entity.h"
class Enemy :
    public Entity
{
public:
	Enemy(sf::Vector2f pos_, EntityID::ID id_,
		bool         isScripted_ = false, EntityScript* script_ = nullptr)
		: Entity(pos_, id_, isScripted_, script_) {}

	virtual void tick(double deltatime) override;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override
	{
		miniSprite->setTextureRect(sf::IntRect(80 + (ID - EntityID::LANDER) * 3, 0, 2, 2));
		Entity::draw(target, states);
	}
};

