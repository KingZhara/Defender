#pragma once
#include "../Projectile.h"

class Bullet :
        public Projectile
{
public:
    explicit Bullet(sf::Vector2f  pos_,
                    bool          isScripted_ = false,
                    EntityScript *script_     = nullptr) :
            Projectile(pos_, EntityID::BULLET, isScripted_, script_) {}

    virtual void tick(double deltatime) override
    {
        Entity::tick(deltatime);

        std::cout << "pos... " << pos.x << ' ' << pos.y << '\n';
    }
};
