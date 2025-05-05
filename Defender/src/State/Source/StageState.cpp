#include "../StageState.h"
#include "../HighscoreState.h"
#include <iostream>

EntityManager StageState::entityManager = EntityManager(false);
char StageState::name[4] = { 0, 0, 0, 0 };
uint8_t StageState::namePos = 0;
const char StageState::validChars[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
bool StageState::SpawnManager::invasion = false;
bool StageState::SpawnManager::spawningComplete = false;
bool StageState::SpawnManager::started = false;
const bool& EntityManager::isInvasion = StageState::SpawnManager::invasion;
const bool& EntityManager::spawningComplete = StageState::SpawnManager::spawningComplete;


bool StageState::playerDead = false;
// Last cutoff multiple where rewards were given
uint32_t StageState::wave = 1;
bool StageState::waveComplete = false;
WaveCompletionScreen* StageState::waveScreen = nullptr;

StageState::StageState()
{
	entityManager = EntityManager();

	playerDead = false;
	wave = 0;
	waveComplete = false;
	
	SpawnManager::reset();
	//entityManager.spawn(EntityID::ASTRONAUT, sf::Vector2f{ 50, 50 });
	//entityManager.spawn(EntityID::POD, sf::Vector2f{ 100, 100 });
	entityManager.spawn(true, EntityID::PLAYER, sf::Vector2f{(float)(DisplayManager::getView().getCenter().x * 1.5) + Entity::makeCenteredTL({0, 0}, EntityID::PLAYER).x, (float)DisplayManager::getView().getCenter().y });
	//entityManager.spawn(EntityID::BOMBER, sf::Vector2f{ 100, 100 });
	//entityManager.spawn(EntityID::BOMBER, sf::Vector2f{ 108, 95 });
	//entityManager.spawn(EntityID::BOMBER, sf::Vector2f{ 90, 90 });
	//entityManager.spawn(EntityID::BOMBER, sf::Vector2f{ 120, 110 });
	//entityManager.spawn(EntityManager::SpawnType::ENEMY, EntityID::SWARMER, sf::Vector2f{ 50, 50});
	//entityManager.spawn(EntityID::SWARMER, sf::Vector2f{ 100, 150 });
	//entityManager.spawn(EntityID::MUTANT, { 50, 50 });

}

bool StageState::tick(Action& actions, double deltatime)
{
	EntityManager::PlayerState state;
	// Validate reset
	if (!playerDead)
	{
		if (!waveComplete)
		{
			waveComplete = EntityManager::waveComplete();

			if (waveComplete)
				waveScreen = new WaveCompletionScreen(EntityManager::getScore(), wave, EntityManager::astronautCount());
		}

		if (waveComplete)
		{
			if (waveScreen->tick(deltatime))
			{
				SpawnManager::nextWave();
				++wave;
				waveComplete = false;
				delete waveScreen;
			}
		}
		else
		{
			state = EntityManager::tick(deltatime, actions);

			//std::cout << "Recieved PlayerState: " << (short)state << '\n';

			// If the player is alive
			if (state == EntityManager::PlayerState::ALIVE)
			{
				playerDead = false;
				SpawnManager::tick(deltatime);

				if (SpawnManager::waveStarted() && EntityManager::astronautCount() == 0) // All astronauts dead
					SpawnManager::startInvasion();
			}
			else if (state == EntityManager::PlayerState::DEAD)
				playerDead = true;
		}
	}
	if (playerDead && SaveHighscore(actions))
	{
		DisplayManager::resetViewPos();
		return true;
	}

	return false;
}

void StageState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    static sf::Text           initials;
    static char               nameStr[4];
    static sf::RectangleShape underline;


    // TODO add timer for death animation
    if (!playerDead)
    {
        UserInterface::drawBackground(target, DisplayManager::getView());
        target.draw(entityManager, states);

    }
    else // Enter initials =------------------------------------------------------------------
    {
        for (int i     = 0; i < 3; i++)
            nameStr[i] = validChars[name[i]];
        nameStr[3] = '\0';

        initials.setString(nameStr);
        initials.setFont(UserInterface::getFont());
        initials.setCharacterSize(32);
        initials.setPosition(DisplayManager::getView().getCenter() - sf::Vector2f(20.f, 10.f));

        underline.setSize(sf::Vector2f(15.f, 2.5));
        // center underline under current character
        underline.setPosition(DisplayManager::getView().getCenter() +
                sf::Vector2f((namePos - 1) * 15.f - 7.5f, 25.f));

        target.draw(initials, states);
        target.draw(underline, states);
    }


	if (waveComplete)
		waveScreen->draw(target, states);
}

std::string StageState::getInitials() {
    static char nameStr[4];
    for (int i     = 0; i < 3; i++)
        nameStr[i] = validChars[name[i]];
    nameStr[3] = '\0';
    return std::string(nameStr);
}

bool StageState::SaveHighscore(Action& actions)
{
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
		{
			HighscoreState::addScore(getInitials().data(), entityManager.getScore());
			

			// Reset ALL data before returning
			leftPressed = false;
			rightPressed = false;
			upPressed = false;
			downPressed = false;
			name[0] = 0;
			name[1] = 0;
			name[2] = 0;
			namePos = 0;

			actions.flags.left = false;

			return true;
		}
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



// Spawn Manager

bool StageState::SpawnManager::firstSub;
uint8_t StageState::SpawnManager::bombCount;
uint8_t StageState::SpawnManager::podCount;
uint8_t StageState::SpawnManager::baiterTime;
uint8_t StageState::SpawnManager::subwave;
uint8_t StageState::SpawnManager::wave;
uint8_t StageState::SpawnManager::invasionWave; // 1 - 4; @ 1, spawn
Timer<double> StageState::SpawnManager::subWaveTimer = Timer<double>(2);




void StageState::SpawnManager::tick(double deltatime)
{
	//std::cout << "Ticking spawn manager: BASE: " << subWaveTimer.BASE << ", TIME: " << subWaveTimer.time << ", INVASION: " << invasion << '\n';
	if (subWaveTimer.tick(deltatime))
	{
		if (!firstSub)
			started = true;
		do
		{
			sf::Vector2<int16_t> target;
			if (firstSub)
			{

				// spawn pods in a range to the right forth of the map based around the player
				// spawn bombers on the same spot at the far right side ~1/6 from the top

				firstSub = false;

				subWaveTimer = Timer<double>(6);
				subWaveTimer.tick(0);

				if (wave == 0)
					resetAstronauts();

				if (subwave == 3)
					baiterTime = 32;

				// Make pod target
				target = {
					(int16_t)((int16_t)(DisplayManager::getView().getCenter().x + COMN::worldSize / 4) % COMN::worldSize),
					(int16_t)(COMN::resolution.y / 2)
				};
				// Spawn pods
				spawnCount(podCount, EntityID::POD, target, { 15, 0 }, { 30, 30 });

				// Make bomb target
				target = {
					(int16_t)((int16_t)(DisplayManager::getView().getCenter().x + COMN::worldSize / 2) % COMN::worldSize),
					(int16_t)(COMN::resolution.y / 6)
				};
				// Spawn bombs
				spawnCount(bombCount, EntityID::BOMBER, target, { 0, 0 }, { 0, 0 });
			}

			if (subwave != 0)
			{

				// Make lander target
				target = {
					0,
					(int16_t)(COMN::resolution.y / 5)
				};
				// Spawn landers
				spawnCount(5, EntityID::LANDER, target, { COMN::worldSize / 3, 0 }, { 200, 5 });
				//std::cout << "Subwave: " << (short)subwave << '\n';
				--subwave;

				if (subwave == 0)
				{
					subWaveTimer = Timer<double>(baiterTime);
					subWaveTimer.tick(0);
					spawningComplete = true;
				}
			}
			else
			{
				baiterTime = std::max(baiterTime / 2, 2);

			    subWaveTimer = Timer<double>(baiterTime);

				subWaveTimer.tick(0);

				//std::cout << "Spawning Baiter";
				spawnCount(1, EntityID::BAITER, { 0, 0 }, { 0, 0 }, { 100, 100 });
			}

			// Loop through all subwaves immediately if it is an invasion
			std::cout << "Subwave: " << (short)subwave << ", Invasion: " << invasion << '\n';
		} while (subwave != 0 && invasion);
	}
}
void StageState::SpawnManager::nextWave()
{
	//std::cout << "Next Wave\n";
	subwave = 4;
	wave = (wave + 1) % 5;
	firstSub = true;
	baiterTime = 16;
	spawningComplete = false;
	started = false;

	EntityManager::waveReset();

	if (invasion)
		++invasionWave;

	if (wave == 0)
		resetAstronauts();

	// I think invasions are actually based around wave being 0, not a seperate wave counter; persisted for 4 waves until astronauts were reset @ n=1
	// This will remain as a placeholder
	if (invasionWave == 5)
	{
		invasionWave = 0;
		invasion = false;

		resetAstronauts();
	}

	if (bombCount == 0 && podCount == 0) // first wave
	{
		podCount = 3;
		bombCount = 3;
	}
	else if (podCount < 4 || bombCount < 5)
	{
		// Alternate incrementing the two
	    if (podCount == bombCount)
			bombCount++;
		else
			podCount++;
	}

	subWaveTimer = Timer<double>(2);
	subWaveTimer.tick(0);
}
void StageState::SpawnManager::reset()
{
	bombCount = 0; podCount = 0; subwave = 3; wave = 0; firstSub = true;
	baiterTime = 32;
	spawningComplete = false;
	started = false;

	// resetAstronauts();

	subWaveTimer = Timer<double>(2);
	subWaveTimer.tick(0);
}

void StageState::SpawnManager::spawnCount(uint8_t count,
        EntityID::ID ID,
        sf::Vector2<int16_t> target,
        sf::Vector2<int16_t> change,
        sf::Vector2<int16_t> entropy)
{

	//std::cout << "Spawning...\n";
    for (uint16_t i = 0; i < count; i++)
    {
		sf::Vector2<int16_t> pos = 
		{
		    (int16_t)((target.x + change.x * i + (entropy.x > 0 ? (rand() % 2 ? 1 : -1) * (rand() % entropy.x) : 0)) % COMN::worldSize),
			(int16_t)((target.y + change.y * i + (entropy.y > 0 ? (rand() % 2 ? 1 : -1) * (rand() % entropy.y) : 0)) % (int16_t)COMN::resolution.y)
		};
		std::cout << "Spawning: " << (int)ID << " at " << pos.x << ", " << pos.y << '\n';
		EntityManager::spawn(true, ID, {(float)pos.x, (float)pos.y});
    }
}

void StageState::SpawnManager::resetAstronauts()
{
	EntityManager::killAstronauts();

	spawnCount(10, EntityID::ASTRONAUT, { 0, (uint16_t)COMN::resolution.y - 20 }, { COMN::worldSize / 5, 0 }, { 10, 0 });
}
