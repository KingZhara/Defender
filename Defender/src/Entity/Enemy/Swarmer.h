#pragma once
#define _USE_MATH_DEFINES
#include "../Enemy.h"
#include <math.h>   // for M_PI


class Swarmer :
    public Enemy
{
public:
    explicit Swarmer(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr);

    virtual ~Swarmer() override = default;

    virtual void tick(double deltatime) override;

private:
    bool init = true, targeted = false;
    float yVal = 0;
    int8_t direction = 0;
    Timer<double> attackTimer{ 0.8 }, switchTimer{ 5 };
};

