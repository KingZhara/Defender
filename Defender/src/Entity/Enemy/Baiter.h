#pragma once
#include "../Enemy.h"
class Baiter :
    public Enemy
{
public:
    explicit Baiter(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr);

    virtual ~Baiter() override = default;

    virtual void tick(double deltaTime) override;

    Timer<double>      attackTimer{ 0.6 };
    Timer<double> targetTimer{ 1 };
};

