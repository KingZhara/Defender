#pragma once
#include "../Enemy.h"
class Pod :
    public Enemy
{
    /*
    The pod gives the player 1000 xp upon death. When a pod is destroyed, it 
    splits into 3 swarmers. It seems to just slowly fly across the screen 
    horizontally or at a shallow angle.
    */
private:

    void tick(double deltaTime);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

