#pragma once
#include "../Projectile.h"
class Bomb :
    public Projectile
{
public:
    explicit Bomb(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr)
        : Projectile(pos_, EntityID::SWARMER, isScripted_, script_)
    {}

    virtual void tick(double deltatime) override
    {
        applyFriction(deltatime, 3);
    }
};

