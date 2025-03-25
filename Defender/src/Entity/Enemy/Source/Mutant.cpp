#include "../Mutant.h"

sf::Shader* Mutant::shader = nullptr;
std::vector<sf::Glsl::Vec3> Mutant::brightColors = { // Data table from somewhere...
	{1.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},
	{1.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 1.0f},
	{1.0f, 0.0f, 1.0f},
	{0.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 1.0f},
	{1.0f, 0.5f, 0.0f}
};

void Mutant::tick(double deltatime)
{
	static Timer<double> replaceType{ 1 / 2. };
	static bool type = false;
	static const sf::Vector2f speed = { 1.5, 1.5 };

	if (replaceType.tick(deltatime))
		type = !type;

	if (type)
		shader->setUniform("ReplaceColor", brightColors[rand() % brightColors.size()]);
	else
		shader->setUniform("ReplaceColor", sf::Glsl::Vec3(0, 0, 0));

	std::cout << "TICKY TYPY" << type << ' ' << '\n';


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
	// @todo implement attacks - is very easy

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

void Mutant::initShader()
{
	shader = new sf::Shader;
	shader->loadFromFile("./res/shader/flashing.frag", sf::Shader::Type::Fragment);

	shader->setUniform("targetColor", sf::Glsl::Vec3{ 136, 0, 255 });
}
