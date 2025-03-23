#pragma once
#include "../Enemy.h"
class Swarmer :
    public Enemy
{
public:
    explicit Swarmer(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr)
        : Enemy(pos_, EntityID::SWARMER, isScripted_, script_)
    {}

    /*
    The swarmer gives the player 150 xp upon death. It's behaviour seems to be 
    heading in the direction of the player and not changing direction until 
    it goes off screen, in that case it repeats the cycle. They also shoot at 
    the player. We should also make sure that they spawn in groups.
    */

    void tick(double deltaTime);
};

