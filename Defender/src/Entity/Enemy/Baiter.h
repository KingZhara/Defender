#pragma once
#include "../Enemy.h"
class Baiter :
    public Enemy
{
public:
    explicit Baiter(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr)
        : Enemy(pos_, EntityID::MUTANT, isScripted_, script_)
    {}

    /*
    The baiter gives the player 200 xp upon death. Baiters fly really fast, 
    but only when they feel like it. They also seem to fly in a random pattern until they decide to fly towards you. They also shoot at you.
    */

    void tick(double deltaTime);
};

