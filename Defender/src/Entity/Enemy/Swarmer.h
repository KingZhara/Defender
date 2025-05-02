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

        targetTimer.tick(0);
    }

    /*
    The swarmer gives the player 150 xp upon death. It's behaviour seems to be 
    heading in the direction of the player and not changing direction until 
    it goes off screen, in that case it repeats the cycle. They also shoot at 
    the player. We should also make sure that they spawn in groups.
    */

    void tick(double deltaTime);

    bool init = true;
    double rot;
    Timer<double> targetTimer{ 1 };
};

