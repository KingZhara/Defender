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
			target.draw(entityManager, states);

			// Needs to reset view to highscore
			sf::View oldView = target.getView();

			sf::View defView = oldView;
			defView.setCenter(defView.getSize().x / 2.f, defView.getSize().y / 2.f);
			target.setView(defView);

			UserInterface::drawBackground(target, DisplayManager::getView());
			UserInterface::drawForeground(target, DisplayManager::getView());


			// Draw player lives
			sf::RectangleShape lifeDisplay;
			lifeDisplay.setTexture(DisplayManager::getTexture());
			lifeDisplay.setTextureRect(Entity::getBounds(EntityID::PLAYER));
			lifeDisplay.setSize(sf::Vector2f(Entity::getBounds(EntityID::PLAYER).getSize()));

			for (int i = 0; i < playerState.lives - 1; i++)
			{
				// Magic numbers are offsets measured by screenshot
				lifeDisplay.setPosition(18 + (Entity::getBounds(EntityID::PLAYER).width + 2) * i, 12);
				target.draw(lifeDisplay);
			}

			// Draw bomb count
			sf::RectangleShape bombDisplay;
			bombDisplay.setTexture(DisplayManager::getTexture());
			bombDisplay.setTextureRect(Entity::getBounds(EntityID::PLAYER));
			bombDisplay.setSize(sf::Vector2f(6, 3)); // size in spritesheet

			for (int i = 0; i < playerState.smart_bombs; i++)
			{
				// Magic numbers are offsets measured by screenshot
				bombDisplay.setPosition(70, 19 + (3 + 1) * i);
				target.draw(bombDisplay);
			}


			// Undo reset view
			target.setView(oldView);
		}
		else
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
