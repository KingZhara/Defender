#pragma once
#include "../Enemy.h"
class Lander :
    public Enemy
{
public:
    explicit Lander(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr)
        : Enemy(pos_, EntityID::LANDER, isScripted_, script_)
    {
        dir = rand() % 2 == 0 ? -1 : 1;

        wanderTimer.tick((rand() % 2 == 0 ? -1 : 1) * rand() % 100 / 10);
    }

    /*
    The lander gives the player 150 xp upon death. Also needs a method to 
    convert into a mutant. Landers also abduct astronauts off the
    ground and turn them into mutants at the top of the screen. The lander's
    movement behaviour involves flying in a straight line across
    the screen and changing altitude randomly, then once it sees an 
    astronaut, it heads straight toward it. They also shoot at the player
    when in range.
    */

    void tick(double deltaTime);
    bool hasTarget();
    bool shouldMutate()
    {
        return pos.y <= COMN::uiHeight + 1 && holding;
    }
	void setTarget(Entity* target);

private:
    int getAstronauts();
	Entity* target = nullptr;
    Timer<double> wanderTimer{ 5, false };
    bool dir = false, holding = false;
};

