#pragma once
#include "../Enemy.h"
class Baiter :
    public Enemy
{
public:
    explicit Baiter(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr)
        : Enemy(pos_, EntityID::BAITER, isScripted_, script_)
    {}

    /*
    The baiter gives the player 200 xp upon death. Baiters fly really fast, 
    but only when they feel like it. They also seem to fly in a random pattern until they decide to fly towards you. They also shoot at you.
    */

    void tick(double deltaTime);

    int16_t destinationX = 0;
    int16_t destinationY = 0;

    bool init = true;
    Timer<double>      attackTimer{ 0.6 };
    Timer<double> targetTimer{ 1 };
    double rot;
};

