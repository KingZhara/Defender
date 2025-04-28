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

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		static sf::Text initials;
		static char nameStr[4];
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
			sf::IntRect playerUI = { 64, 18, 10, 4 };
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
			sf::IntRect bombUI = { 75, 18, 6, 4 };
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
			for (int i = 0; i < 3; i++)
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

	static std::string getInitials()
	{
		static char nameStr[4];
		for (int i = 0; i < 3; i++)
			nameStr[i] = validChars[name[i]];
		nameStr[3] = '\0';
		return std::string(nameStr);
	}

private:
	static bool SaveHighscore(Action& actions);

	static EntityManager entityManager;
	static Timer<double> hyperspaceCooldown;
	static PlayerState playerState;
	static char name[4];
	static uint8_t namePos;

	static const char validChars[];
};
