#pragma once
#include "../Enemy.h"
class Swarmer :
    public Enemy
{
    /*
    The swarmer gives the player 150 xp upon death. It's behaviour seems to be 
    heading in the direction of the player and not changing direction until 
    it goes off screen, in that case it repeats the cycle. They also shoot at 
    the player. We should also make sure that they spawn in groups.
    */
};

