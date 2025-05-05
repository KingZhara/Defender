#include "../Lander.h"

void Lander::tick(double deltatime)
{
	// Ricky will handle logic for picking an astronaut

	// When no astronauts are available, make wander based on a timer; assume can check with "getAstronaut()"; if it returns -1, no astronauts are available
	if (wanderTimer.isComplete())
	{
		if (target)
		{
			if (!holding)
			{
				if (!collide(target))
					vel = makeTargetedVec(pos, ID, target, 1, true).vel;
				else
					holding = true;
			}

			if (holding)
			{
				vel = getEVel(ID);
				vel.x = 0;

				target->setPos({ pos.x + makeCenteredTL(pos, ID).x - makeCenteredTL(target->getPos(), EntityID::ASTRONAUT).x / 2, pos.y - (getBounds(ID).height + 2) });
			}
			// if held, move upwards, set target pos to {pos.x + this center - half its center, pos.y - (this height + 2)}
		}
		else
		{
			dir = rand() % 2 == 0 ? -1 : 1;

			wanderTimer.tick((rand() % 2 == 0 ? -1 : 1) * rand() % 100 / 10);
		}
	}
	else
	{
		wanderTimer.tick(deltatime);

		vel = getEVel(ID);

		// Should use a height map
		if (pos.y > COMN::resolution.y / 4 * 3)
			vel.y = 0;
		if (dir)
			vel.x *= -1;
	}
	Enemy::tick(deltatime);
}

bool Lander::hasTarget() { return target; }

void Lander::setTarget(Entity *target_)
{
	target = target_;
}

int Lander::getAstronauts()
{
	return -1;
}
