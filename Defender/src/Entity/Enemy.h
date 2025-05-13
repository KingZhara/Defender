#pragma once
#include "Entity.h"
class Enemy : public Entity
{
public:
	Enemy(sf::Vector2f pos_, EntityID::ID id_,
		bool         isScripted_ = false, EntityScript* script_ = nullptr);

    virtual ~Enemy() override = default;

	virtual void tick(double deltatime) override;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

