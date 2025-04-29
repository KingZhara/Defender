#include "../StageState.h"
#include "../HighscoreState.h"
#include <iostream>

EntityManager StageState::entityManager = EntityManager(false);
Timer<double> StageState::hyperspaceCooldown = Timer<double>(5.0f /*@todo correct time in seconds*/, false);
StageState::PlayerState StageState::playerState = PlayerState();
char StageState::name[4] = { 0, 0, 0, 0 };
uint8_t StageState::namePos = 0;

const char StageState::validChars[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

StageState::StageState()
{

	entityManager = EntityManager();
	entityManager.spawn(EntityManager::SpawnType::ASTRONAUT, EntityID::ASTRONAUT, sf::Vector2f{ 50, 50 });
	//entityManager.spawn(EntityManager::SpawnType::ENEMY, EntityID::POD, sf::Vector2f{ 100, 100 });
	entityManager.spawn(EntityManager::SpawnType::PLAYER, EntityID::PLAYER, sf::Vector2f{(float)(DisplayManager::getView().getCenter().x * 1.5) + Entity::makeCenteredTL({0, 0}, EntityID::PLAYER).x, (float)DisplayManager::getView().getCenter().y });
	entityManager.spawn(EntityManager::SpawnType::ENEMY, EntityID::BOMBER, sf::Vector2f{ 100, 100 });
	//entityManager.spawn(EntityManager::SpawnType::ENEMY, EntityID::SWARMER, sf::Vector2f{ 50, 50});
	entityManager.spawn(EntityManager::SpawnType::ENEMY, EntityID::SWARMER, sf::Vector2f{ 100, 150 });
	entityManager.spawn(EntityManager::SpawnType::ENEMY, EntityID::MUTANT, { 50, 50 });
}

bool StageState::tick(Action& actions, double deltatime)
{
	static bool playerDead = false;
	// For this to be adjustable it should be moved to the class definition with appropriate methods
	static const ScoreType rewardReq = 10000;
	// Last cutoff multiple where rewards were given
	static uint16_t lastReward = 1;

	static Timer<double> spwn{ 1 };


	if (spwn.tick(deltatime))
	{
		//entityManager.spawn(EntityManager::SpawnType::ENEMY, EntityID::MUTANT, sf::Vector2f{ 50, 50 });
	}


	if (!playerDead)
	{
		// ##############################
        // ####  Handle  Keypresses  ####
		// ##############################

		// @todo check if a cooldown was needed, I am just assuming it is - Ricky
		// Handle the hyperspace cooldown
		if (!hyperspaceCooldown.isComplete())
			hyperspaceCooldown.tick(deltatime);

		// Execute hyperspace if applicable
		if (actions.flags.hyperspace && hyperspaceCooldown.isComplete())
		{
			entityManager.hyperspace(DisplayManager::getView().getSize(), DisplayManager::getView().getCenter().x - (DisplayManager::getView().getSize().x / 2.f));
			actions.flags.hyperspace = false;
			hyperspaceCooldown.tick(0);
		}

		// Handle and update smart bombs accordingly
		if (actions.flags.smart_bomb && playerState.smart_bombs > 0)
		{
			entityManager.killArea(DisplayManager::getView().getViewport());
			--playerState.smart_bombs;
		}

		// ##############################
		// #######  Handle Score  #######
		// ##############################

		if (entityManager.getScore() >= rewardReq * lastReward)
		{
			++lastReward;

			if (playerState.smart_bombs != 3)
				++playerState.smart_bombs;
			if (playerState.lives != 3)
				++playerState.lives;
		}

		// Handle player death.
		playerDead = entityManager.tick(deltatime, DisplayManager::getView().getCenter().x);
	}

	// Should handle saving the high score if needed
	if (playerDead)
	{
		if (playerState.lives <= 1)
		{
			playerState.lives = 0;
			if (SaveHighscore(actions))
			{
				// Reset ALL data before returning
				playerState.lives = 3;
				playerState.smart_bombs = 0;
				playerDead = false;

				DisplayManager::resetViewPos();
				return true;
			}

		    return false;
		}

		std::cout << "KILLING>>>\n";
		--playerState.lives;


		//@todo Add respawning mechanics...
		playerDead = false;
	}
	else
		EntityManager::adjViewport(&DisplayManager::getView(), deltatime);

	return false;
}

void StageState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    static sf::Text           initials;
    static char               nameStr[4];
    static sf::RectangleShape underline;

    // TODO add timer for death animation
    if (playerState.lives > 0)
    {
        UserInterface::drawBackground(target, DisplayManager::getView());
        target.draw(entityManager, states);

        // Needs to reset view to highscore
        sf::View oldView = target.getView();

        sf::View defView = oldView;
        defView.setCenter(defView.getSize().x / 2.f, defView.getSize().y / 2.f);
        target.setView(defView);


        // Need to cover stuff from gameplay like projectiles ---------------------------------------------
        sf::RectangleShape clearUI;
        clearUI.setFillColor(sf::Color::Black);
        clearUI.setSize(sf::Vector2f(COMN::resolution.x, COMN::uiHeight));
        target.draw(clearUI, states);


        UserInterface::drawForeground(target, DisplayManager::getView());

        // Draw player lives ---------------------------------------------------------------------
        sf::IntRect  playerUI    = { 64, 18, 10, 4 };
        sf::Vector2i playerUIPos = { 18, 12 };

        sf::RectangleShape lifeDisplay;
        lifeDisplay.setTexture(DisplayManager::getTexture());
        lifeDisplay.setTextureRect(playerUI);
        lifeDisplay.setSize(sf::Vector2f(playerUI.getSize()));

        for (int i = 0; i < playerState.lives - 1; i++)
        {
            lifeDisplay.setPosition(playerUIPos.x + (playerUI.width + 2) * i, playerUIPos.y);
            target.draw(lifeDisplay);
        }

        // Draw bomb count ---------------------------------------------------------------------
        sf::IntRect  bombUI    = { 75, 18, 6, 4 };
        sf::Vector2i bombUIPos = { 70, 19 };

        sf::RectangleShape bombDisplay;
        bombDisplay.setTexture(DisplayManager::getTexture());
        bombDisplay.setTextureRect(bombUI);
        bombDisplay.setSize(sf::Vector2f(bombUI.getSize()));

        for (int i = 0; i < playerState.smart_bombs; i++)
        {
            bombDisplay.setPosition(bombUIPos.x, bombUIPos.y + (bombUI.height + 1) * i);
            target.draw(bombDisplay);
        }


        // draw screen view markers -------------------------------------------------
        constexpr int screenMarkerWidth = 15;

        sf::RectangleShape screenMarkerMain;
        screenMarkerMain.setSize(sf::Vector2f(screenMarkerWidth, 2));
        screenMarkerMain.setFillColor(sf::Color(0xBFBFBFFF));


        sf::RectangleShape screenMarkerSide;
        screenMarkerSide.setSize(sf::Vector2f(1, 4));
        screenMarkerSide.setFillColor(sf::Color(0xBFBFBFFF));


        constexpr float minimapScale = 0.05;

        // top marker
        screenMarkerMain.setPosition(COMN::resolution.x / 2 - screenMarkerWidth / 2.f, 0);

        target.draw(screenMarkerMain, states);
			
        screenMarkerSide.setPosition(screenMarkerMain.getPosition().x, 0);
        target.draw(screenMarkerSide, states);
			
        screenMarkerSide.setPosition(screenMarkerMain.getPosition().x + screenMarkerWidth, 0);
        target.draw(screenMarkerSide, states);

        // bottom marker
        screenMarkerMain.setPosition(screenMarkerMain.getPosition().x, COMN::uiHeight);
        target.draw(screenMarkerMain, states);
			
        screenMarkerSide.setPosition(screenMarkerMain.getPosition().x, COMN::uiHeight - 2);
        target.draw(screenMarkerSide, states);
			
        screenMarkerSide.setPosition(screenMarkerMain.getPosition().x + screenMarkerWidth, COMN::uiHeight - 2);
        target.draw(screenMarkerSide, states);

        // Show right aligned score ------------------------------------------------------------
        // Same position as in highscore
        sf::Text scoreTxt;
        scoreTxt.setFont(UserInterface::getFont());
        scoreTxt.setCharacterSize(16);
        scoreTxt.setFillColor(sf::Color(COMN::ShaderTarget));
        scoreTxt.setString(std::to_string(entityManager.getScore()));
        scoreTxt.setOrigin(scoreTxt.getGlobalBounds().width, 0);
        scoreTxt.setPosition(63, 14);
        target.draw(scoreTxt, states);


        // Undo reset view
        target.setView(oldView);
    }
    else // Enter initials =------------------------------------------------------------------
    {
        for (int i     = 0; i < 3; i++)
            nameStr[i] = validChars[name[i]];
        nameStr[3] = '\0';

        initials.setString(nameStr);
        initials.setFont(UserInterface::getFont());
        initials.setCharacterSize(32);
        initials.setPosition(DisplayManager::getView().getCenter() - sf::Vector2f(25.f, 10.f));

        underline.setSize(sf::Vector2f(15.f, 2.5));
        // center underline under current character
        underline.setPosition(DisplayManager::getView().getCenter() +
                sf::Vector2f((namePos - 1) * 20.f - 7.5f, 25.f));

        target.draw(initials, states);
        target.draw(underline, states);
    }
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