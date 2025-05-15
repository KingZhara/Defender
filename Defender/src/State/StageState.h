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
	// For shared variables
	friend class EntityManager;

public:
	StageState();

	~StageState() = default;

    static bool tick(Action& actions, double deltatime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    static std::string getInitials();

private:
	class SpawnManager;
	friend class UserInterface;

	static bool SaveHighscore(Action& actions);

	static EntityManager         entityManager;
	static char                  name[4];
	static uint8_t               namePos;
	static bool                  playerDead;
	static uint32_t              wave;
	static bool                  waveComplete;
	static WaveCompletionScreen* waveScreen;
	static const char            validChars[];
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
	static void startInvasion()
	{
		invasion = true; baiterTime = 45;
		subWaveTimer.addTime(subWaveTimer.getBase() - subWaveTimer.time);
	}
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
