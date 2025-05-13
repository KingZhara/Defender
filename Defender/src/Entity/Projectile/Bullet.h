#pragma once
#include "../Projectile.h"

class Bullet :
        public Projectile
{
public:
    explicit Bullet(sf::Vector2f  pos_,
                    bool          isScripted_ = false,
                    EntityScript *script_     = nullptr);

    virtual ~Bullet() override = default;

    virtual void tick(double deltatime) override;
};
