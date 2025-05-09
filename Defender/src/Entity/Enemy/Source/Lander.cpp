#include "../Lander.h"
#include "../../Astronaut.h"

void Lander::tick(double deltatime)
{
	static Entity* tracked = this;
	// Ricky will handle logic for picking an astronaut

	// When no astronauts are available, make wander based on a timer; assume can check with "getAstronaut()"; if it returns -1, no astronauts are available
	if (wanderTimer.isComplete())
	{
		if (target)
		{
			if (!holding)
			{
				if (this == tracked)
    				std::cout << "Chasing target ";
				if (!collide(target))
					vel = makeTargetedVec(pos, ID, target, 1, true).vel;
				else
				{
					holding = true;
					// Garanteed to be an astronaut
					dynamic_cast<Astronaut*>(target)->setHolder(this);
				}
			}
			
			if (holding)
			{
				if (this == tracked)
					std::cout << "Hasg target ";
				vel = getEVel(ID);
				vel.x = 0;
				vel.y *= -1;
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
		if (this == tracked)
			std::cout << "Wandering ";
		wanderTimer.tick(deltatime);

		vel = getEVel(ID);

		// @todo smooth with sinusoidal function
		if (pos.y > COMN::resolution.y - (UserInterface::getHeight(pos.x) + 35)) // 40 - 35 = 5px range
			vel.y *= -1;
		else if (pos.y > COMN::resolution.y - (UserInterface::getHeight(pos.x) + 40)) // Hover 40px above the ground
			vel.y = 0;


		if (dir)
			vel.x *= -1;
	}

	if (this == tracked)
		std::cout << " BEF: { VEL: (" << vel.x << ", " << vel.y << "), POS: (" << pos.x << ", " << pos.y << ") }";
	Enemy::tick(deltatime);
	if (holding)
		target->setPos({ makeCenteredTL(pos, ID).x - makeCenteredTL({0, 0}, EntityID::ASTRONAUT).x / 2, pos.y + (getBounds(ID).height + 2) });
	if (this == tracked)
		std::cout << " AFT: { VEL: (" << vel.x << ", " << vel.y << "), POS: (" << pos.x << ", " << pos.y << ") }\n";
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
