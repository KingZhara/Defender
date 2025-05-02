#pragma once
#include "../Enemy.h"
class Pod :
    public Enemy
{
public:
    explicit Pod(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr)
        : Enemy(pos_, EntityID::POD, isScripted_, script_)
    {
        rot = ((rand() % 120) - 60) + (180 * (rand() % 2));

        vel.x = (float)(43 * cos(rot * (3.141592653589 / 180)));
        vel.y = (float)(43 * sin(rot * (3.141592653589 / 180)));
    }

    ~Pod()
	{
		std::cout << "Pod destroyed" << std::endl;
	}

    virtual void tick(double deltatime) override
    {
		std::cout << "Pod tick" << std::endl;

        Enemy::tick(deltatime);
    }

    /*
    The pod gives the player 1000 xp upon death. When a pod is destroyed, it 
    splits into 3 swarmers. It seems to just slowly fly across the screen 
    horizontally or at a shallow angle.
    */
    //in degrees
    int16_t rot;

    //int16_t rot = 45;
};

