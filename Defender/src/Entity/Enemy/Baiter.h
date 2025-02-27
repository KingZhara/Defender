#pragma once
#include "../Enemy.h"
class Baiter :
    public Enemy
{
    /*
    The baiter gives the player 200 xp upon death. Baiters fly really fast, 
    but only when they feel like it. They also seem to fly in a random pattern until they decide to fly towards you. They also shoot at you.
    */

private:

    void tick(double deltaTime);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

