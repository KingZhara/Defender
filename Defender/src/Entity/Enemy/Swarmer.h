#pragma once
#define _USE_MATH_DEFINES
#include "../Enemy.h"
#include <math.h>   // for M_PI


class Swarmer :
    public Enemy
{
public:
    explicit Swarmer(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr)
        : Enemy(pos_, EntityID::SWARMER, isScripted_, script_)
    {
        vel = getEVel(ID);
        double rot = static_cast<double>(rand()) / RAND_MAX * 2.0f * M_PI;

        vel.x *= std::cos(rot);
        vel.y *= std::sin(rot);

        attackTimer.tick(0);
		switchTimer.tick(4.5);
    }
    virtual void tick(double deltatime) override;

    bool init = true;
    float yVal = 0;
    int8_t direction = 0;
    Timer<double> attackTimer{ 0.8 }, switchTimer{ 5 };
};

