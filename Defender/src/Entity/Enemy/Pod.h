#pragma once
#include "../Enemy.h"
class Pod :
    public Enemy
{
public:
    explicit Pod(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr)
        : Enemy(pos_, EntityID::POD, isScripted_, script_)
    {}

    /*
    The pod gives the player 1000 xp upon death. When a pod is destroyed, it 
    splits into 3 swarmers. It seems to just slowly fly across the screen 
    horizontally or at a shallow angle.
    */

    void tick(double deltaTime);
    //in degrees
    int16_t rot = (rand()%30);

    //int16_t rot = 45;
};

