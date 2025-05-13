#pragma once
#include "../Projectile.h"
class Bomb :
    public Projectile
{
public:
    explicit Bomb(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr);

    virtual ~Bomb() override = default;

    virtual void tick(double deltatime) override;
};

