#include "../Swarmer.h"

void Swarmer::tick(double deltatime)
{

    if (switchTimer.tick(deltatime))
    {
        EntityTarget target = makePlayerTargetedVec(pos, ID, 1, true);

        if (direction == 0)
            direction = 1;
        direction = std::copysign(direction, target.pos.x);

        targeted = isOnScreen();

        if (targeted)
			switchTimer.tick(4.5); // Reset to .5 sec left til target resets
        else
        {
            if (switchTimer.tick((rand() % 2 == 0 ? -1 : 1) * rand() % 50 / 10))
                yVal = 0;
        }
        vel = target.vel;
    }


    if (direction != 0 && !targeted)
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
 