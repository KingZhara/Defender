#include "../Mutant.h"

void Mutant::tick(double deltatime)
{
	static const sf::Vector2f speed = { 1.5, 1.5 };

	// unless the mutants are directly above or below the player, they aim to be around 1/8 of the screen above or below the player
	// The mutants bob up and down every other frame; up, nul, up, nul, down, nul, down, nul, repeat
	// For bobbing, we will approximate to once every 4 frames instead of every frame~ 15fps conversion

	// Bobbing
	if (bobbing.tick(deltatime))
	{
		// Bob up or down on even intervals
		if (bobStage % 2 == 0)
		{
			if (bobStage <= 3)
				pos.y += 2;
			else
				pos.y -= 2;
		}

		// Update
		bobStage = (bobStage + 1) % 8;
	}


	// Attack

	// Movement
	// If above the player
	if (abs(pos.x - playerPos->x) < 4) // 2 = square radius on the x-axis around the player
	{
		if (pos.x - playerPos->x < -2)
			vel.x = speed.x;
		else if(pos.x - playerPos->x > 2)
			vel.x = -speed.x;

		if (pos.y - playerPos->y < 0)
			vel.y = speed.y;
		else
			vel.y = -speed.y;
	}
	else
	{
		if (pos.x - playerPos->x < 0)
			vel.x = speed.x;
		else
			vel.x = -speed.x;

		std::cout << abs(pos.y - playerPos->y) << " > " << 240 / 4 + 5 << '\n';

		// If we are out of the ideal range (screen height / 8 +- 5)
		if (abs(pos.y - playerPos->y) > 240 / 4 + 5 || abs(pos.y - playerPos->y) < 240 / 4 - 5)
		{
			std::cout << "A>>>>>" << '\n';

			// If above the range, move down
			if (abs(pos.y - playerPos->y) > 240 / 4 + 5)
				vel.y = +speed.y;
			else
				vel.y = -speed.y;
		}
		else
			vel.y = 0;
	}

	Entity::tick(deltatime);
}