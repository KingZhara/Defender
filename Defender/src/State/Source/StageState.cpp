#include "../StageState.h"
#include <iostream>

sf::View* StageState::viewport = nullptr;
EntityManager StageState::entityManager = EntityManager(false);
Timer<double> StageState::hyperspaceCooldown = Timer<double>(5 /*@todo correct time in seconds*/, true);
StageState::PlayerState StageState::playerState = PlayerState();
char StageState::name[3] = { 'A', 'A', 'A' };
sf::Text StageState::initials;

StageState::StageState()
{
	entityManager.spawn(EntityManager::SpawnType::PLAYER, { 250, 150 }, EntityID::PLAYER);
	entityManager.spawn(EntityManager::SpawnType::ASTRONAUT, { 50, 50 }, EntityID::ASTRONAUT);
	entityManager.spawn(EntityManager::SpawnType::ENEMY, { 50, 50 }, EntityID::MUTANT);
	entityManager.spawn(EntityManager::SpawnType::ENEMY, { 50, 50 }, EntityID::MUTANT);
	entityManager.spawn(EntityManager::SpawnType::ENEMY, { 50, 50 }, EntityID::MUTANT);
}

bool StageState::tick(Action& actions, double deltatime)
{
	static bool playerDead = false;

	// For this to be adjustable it should be moved to the class definition with appropriate methods
	static const ScoreType rewardReq = 10000;
	// Last cutoff multiple where rewards were given
	static uint16_t lastReward = 1;

	if (!playerDead)
	{
		// @todo check if a cooldown was needed, I am just assuming it is - Ricky
		// Handle the hyperspace cooldown
		if (!hyperspaceCooldown.isComplete())
			hyperspaceCooldown.tick(deltatime);

		// Execute hyperspace if applicable
		if (actions.flags.hyperspace && hyperspaceCooldown.isComplete())
			entityManager.hyperspace(viewport->getViewport());

		// Handle and update smart bombs accordingly
		if (actions.flags.smart_bomb && playerState.smart_bombs > 0)
		{
			entityManager.killArea(viewport->getViewport());
			--playerState.smart_bombs;
		}

		if (entityManager.getScore() >= rewardReq * lastReward)
		{
			++lastReward;

			if (playerState.smart_bombs != 3)
				++playerState.smart_bombs;
			if (playerState.lives != 3)
				++playerState.lives;
		}

		// Handle player death.
		playerDead = entityManager.tick(actions, deltatime);
	}
	// Should handle saving the high score if needed
	if (playerDead)
	{
		if (playerState.lives <= 1)
		{
			playerState.lives = 0;
			return SaveHighscore(actions);
		}

		--playerState.lives;
	}

	// else
	playerDead = false;

	return false;
}

bool StageState::SaveHighscore(Action& actions)
{
	static bool validScore = false;

	// Utilize static methods in HighscoreState to handle checking if the score is applicable, and then saving the score

	// if high score is applicable
	//     change key @ pos; 1-3

	// if complete
	//     utilize static method in HighscoreState to save score

	static uint8_t pos = 0;
	static int fontSize = 30;

	if (actions.flags.down)
	{
		fontSize--;
		//name[pos]++;
		//std::cout << pos << " " << name << std::endl;
	}
	else if (actions.flags.up)
		fontSize++;
		//name[pos]--;
	initials.setCharacterSize(fontSize);
	/*
	else if (actions.flags.left)
		pos--;
	else if (actions.flags.thrust)
		pos++;*/

	return false;
}
