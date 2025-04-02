#include "../StageState.h"
#include <iostream>

EntityManager StageState::entityManager = EntityManager(false);
Timer<double> StageState::hyperspaceCooldown = Timer<double>(5 /*@todo correct time in seconds*/, true);
StageState::PlayerState StageState::playerState = PlayerState();
char StageState::name[3] = { 0, 0, 0 };
uint8_t StageState::namePos = 0;

const char StageState::validChars[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

StageState::StageState()
{
	entityManager = EntityManager();
	entityManager.spawn(EntityManager::SpawnType::PLAYER, { 250, 150 }, EntityID::PLAYER);
	entityManager.spawn(EntityManager::SpawnType::ASTRONAUT, { 50, 50 }, EntityID::ASTRONAUT);
	//entityManager.spawn(EntityManager::SpawnType::ENEMY, { 100, 100 }, EntityID::BAITER);
	//entityManager.spawn(EntityManager::SpawnType::ENEMY, { 50, 100 }, EntityID::LANDER);
	entityManager.spawn(EntityManager::SpawnType::ENEMY, { 50, 50 }, EntityID::MUTANT);
	//entityManager.spawn(EntityManager::SpawnType::ENEMY, { 50, 50 }, EntityID::MUTANT);
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
			entityManager.hyperspace(DisplayManager::getView().getSize(), DisplayManager::getView().getCenter().x - (DisplayManager::getView().getSize().x / 2.f));

		// Handle and update smart bombs accordingly
		if (actions.flags.smart_bomb && playerState.smart_bombs > 0)
		{
			entityManager.killArea(DisplayManager::getView().getViewport());
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
		playerDead = entityManager.tick(actions, deltatime, DisplayManager::getView().getCenter().x);
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


		//@todo Add respawning mechanics...
		playerDead = false;
	}
	else
		EntityManager::adjViewport(&DisplayManager::getView());

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

	static bool leftPressed = false;
	static bool rightPressed = false;
	static bool upPressed = false;
	static bool downPressed = false;

	if (actions.flags.down && !downPressed)
	{
		if (validChars[++name[namePos]] == '\0')
			name[namePos] = 0;
	}
	else if (actions.flags.up && !upPressed)
	{
		if (validChars[name[namePos]] == ' ')
			name[namePos] = sizeof(validChars) - 2;
		else
			name[namePos]--;
	}
	else if (actions.flags.leftHS && !leftPressed)
	{
		if (namePos < 2)
			namePos++;
		else
			return true;
	}
	else if (actions.flags.thrust && !rightPressed)
	{
		if (namePos > 0)
			namePos--;
	}

	leftPressed = actions.flags.leftHS;
	rightPressed = actions.flags.thrust;
	upPressed = actions.flags.up;
	downPressed = actions.flags.down;

	return false;
}
