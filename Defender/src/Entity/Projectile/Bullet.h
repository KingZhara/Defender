#pragma once
#include "../Projectile.h"
class Bullet :
    public Projectile
{
public:
    explicit Bullet(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr)
        : Projectile(pos_, EntityID::SWARMER, isScripted_, script_)
    {}
};

