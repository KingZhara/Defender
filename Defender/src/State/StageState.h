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
		uint8_t smart_bombs : 2 = 0;
		uint8_t padding : 4 = 0;
	};

public:
	StageState();

	~StageState() = default;

	bool tick(Action& actions, double deltatime);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		static sf::Text initials;
		static char nameStr[4];
		static sf::RectangleShape underline;

		// TODO add timer for death animation
		if (playerState.lives > 0) 
			target.draw(entityManager, states);
		else
		{
			nameStr[0] = validChars[name[0]];
			nameStr[1] = validChars[name[1]];
			nameStr[2] = validChars[name[2]];
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

	std::string getInitials()
	{
		static char nameStr[4];
		nameStr[0] = validChars[name[0]];
		nameStr[1] = validChars[name[1]];
		nameStr[2] = validChars[name[2]];
		nameStr[3] = '\0';
		return std::string(nameStr);
	}

private:
	static bool SaveHighscore(Action& actions);


	static EntityManager entityManager;
	static Timer<double> hyperspaceCooldown;
	static PlayerState playerState;
	static char name[3];
	static uint8_t namePos;

	static const char validChars[];
};
