#pragma once

#include <SFML/Graphics.hpp>

#include "../Utility/WaveCompletionScreen.h"

#include "../Utility/Action.h"
#include "../Utility/EntityManager.h"
#include "../Utility/common.h"
#include "../Utility/UserInterface/UserInterface.h"
#include "../Utility/DisplayManager.h"

class StageState : public sf::Drawable
{
	// For shared variable
	friend class EntityManager;

	struct PlayerState
	{
		uint8_t lives = 3;
		uint8_t smart_bombs = 3;
	};

public:
	StageState();

	~StageState() = default;

    static bool tick(Action& actions, double deltatime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    static std::string getInitials();

	static int getPlayerLives() { return playerState.lives; }
	static int getPlayerBombs() { return playerState.smart_bombs; }
	static int getScore() { return entityManager.getScore(); }

	static const EntityManager& getEntityManager() { return entityManager; }

private:
	class SpawnManager;

	static bool SaveHighscore(Action& actions);

	static EntityManager entityManager;
	static Timer<double> hyperspaceCooldown;
	static PlayerState playerState;
	static char name[4];
	static uint8_t namePos;

	static bool playerDead;
	// For this to be adjustable it should be moved to the class definition with appropriate methods
	static constexpr ScoreType rewardReq = 10000;
	// Last cutoff multiple where rewards were given
	static uint16_t lastReward;
	static uint32_t wave;
	static bool waveComplete;

	static WaveCompletionScreen* waveScreen;

	static const char validChars[];
};

// ################# STAGE STATE END #####################




// ################# SPAWN MANAGER ######################
class StageState::SpawnManager
{
	// for Shared Variable
	friend class EntityManager;

public:
	SpawnManager() = delete;

	static void tick(double deltatime);
	static void nextWave();
	static void reset();
	static void startInvasion() { invasion = true; baiterTime = 45; }
	static bool waveStarted() { return started; };

private:
    /**
     * Spawns a group of entities of the specified ID and parameters
     * 
     * @param count Number of entities
     * @param ID Type of entity
	 * @param target Target position
	 * @param change Difference in position between each entity spawn
     * @param entropy Entropy for random position offset
     */
    static void spawnCount(uint8_t count, EntityID::ID ID, sf::Vector2<int16_t> target, sf::Vector2<int16_t> change, sf::Vector2<int16_t> entropy);
    static void resetAstronauts();

	static bool firstSub;
	static bool invasion;
	static bool started;
	static bool spawningComplete;
	static uint8_t bombCount, podCount, baiterTime;
	static Timer<double> subWaveTimer;
	static uint8_t subwave;
	static uint8_t wave, invasionWave; // 1 - 4; @ 1, spawn
 };
