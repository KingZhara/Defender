#pragma once
#include "../Enemy.h"
class Mutant :
    public Enemy
{
    /*
    The mutant gives the player 150 xp upon death. Mutants just fly 
    horizontally until the player gets within range, then they fly directly 
    toward the player and shoots at them , always going horizontally before 
    they move vertically.
    */

    virtual bool tick();
};

