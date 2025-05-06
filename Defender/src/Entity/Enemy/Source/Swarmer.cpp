#include "../Swarmer.h"

void Swarmer::tick(double deltatime)
{
    if (switchTimer.tick(deltatime))
    {
        direction = rand() % 2 == 0 ? -1 : 1;

        if (switchTimer.tick((rand() % 2 == 0 ? -1 : 1) * rand() % 50 / 10))
            yVal = 0;
    }


    if (direction != 0)
    {
        yVal = fmod(yVal + deltatime, 2);
        vel = getEVel(ID);

        vel.y *= std::cos(M_PI * yVal);
        vel.x *= direction;
    }

    if (attackTimer.tick(deltatime))
        entityQueue.emplace(QueuedEntity(makeCenteredTL(pos, ID), EntityID::BULLET));

    Enemy::tick(deltatime);
}
 