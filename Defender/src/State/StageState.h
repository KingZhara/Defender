#pragma once

#include <SFML/Graphics.hpp>
#include "../Utility/Action.h"
#include "../Utility/EntityManager.h"
#include "../Utility/common.h"
#include "../Utility/UserInterface/UserInterface.h"
#include "../Utility/DisplayManager.h"

class StageState : public sf::Drawable
{
	struct PlayerState
	{
		uint8_t lives : 2 = 3;
		uint8_t smart_bombs : 2 = 3;
		uint8_t padding : 4 = 0;
	};

public:
	StageState();

	~StageState() = default;

    static bool tick(Action& actions, double deltatime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    static std::string getInitials();

private:
	class SpawnManager;

	static bool SaveHighscore(Action& actions);

	static EntityManager entityManager;
	static Timer<double> hyperspaceCooldown;
	static PlayerState playerState;
	static char name[4];
	static uint8_t namePos;

	static const char validChars[];
};

class StageState::SpawnManager
{
public:
	SpawnManager() = delete;

	void tick();

	void setWave(uint16_t);

private:
    /**
     * Spawns a group of entities of the specified ID and parameters
     * 
     * @param count Number of entities
     * @param height The target height
     * @param width The x-spacing
     * @param ID ID of the entities
     */
    void spawnCount(uint8_t count, uint8_t height, uint16_t width, EntityID::ID ID);
	sf::Vector2<uint16_t> genPos(uint8_t x, uint8_t y);
    void resetRNG();

};
