#pragma once
#include "../Enemy.h"
class Lander :
    public Enemy
{
public:
    explicit Lander(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr)
        : Enemy(pos_, EntityID::LANDER, isScripted_, script_)
    {}

    /*
    The lander gives the player 150 xp upon death. Also needs a method to 
    convert into a mutant. Landers also abduct astronauts off the
    ground and turn them into mutants at the top of the screen. The lander's
    movement behaviour involves flying in a straight line across
    the screen and changing altitude randomly, then once it sees an 
    astronaut, it heads straight toward it. They also shoot at the player
    when in range.
    */

private:

    void tick(double deltaTime);
};

