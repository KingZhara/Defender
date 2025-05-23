#include "../Lander.h"
#include "../../Astronaut.h"

Lander::Lander(sf::Vector2f pos_, bool isScripted_, EntityScript *script_): Enemy(pos_, EntityID::LANDER, isScripted_, script_)
{
    dir = rand() % 2 == 0 ? -1 : 1;

    wanderTimer.tick((rand() % 2 == 0 ? -1 : 1) * rand() % 100 / 10);
}

void Lander::tick(double deltatime)
{
	static Entity* tracked = this;
	// Ricky will handle logic for picking an astronaut

	if (wanderTimer.isComplete())
	{
		if (target)
		{
			if (!holding)
			{
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

	// Fire
	if (attackTimer.tick(deltatime))
		entityQueue.emplace(QueuedEntity(makeCenteredTL(pos, ID), EntityID::BULLET));


	Enemy::tick(deltatime);

	if (holding && target)
		target->setPos({ makeCenteredTL(pos, ID).x - makeCenteredTL({0, 0}, EntityID::ASTRONAUT).x / 2, pos.y + (getBounds(ID).height + 2) });
}

bool Lander::hasTarget() { return target; }
bool Lander::shouldMutate() {
    return pos.y <= COMN::uiHeight + 1 && holding;
}

void Lander::setTarget(Entity *target_)
{
	if (target_ == nullptr)
		holding = false;
	target = target_;
}
