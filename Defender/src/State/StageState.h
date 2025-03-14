#pragma once

#include <SFML/Graphics.hpp>
#include "../Utility/Action.h"
#include "../Utility/EntityManager.h"
#include "../Utility/UserInterface/UserInterface.h"

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
		nameStr[0] = validChars[name[0]];
		nameStr[1] = validChars[name[1]];
		nameStr[2] = validChars[name[2]];
		nameStr[3] = '\0';

		initials.setString(nameStr);
		initials.setFont(UserInterface::getFont());
		initials.setCharacterSize(30);
		initials.setPosition(viewport->getCenter() - initials.getGlobalBounds().getSize() / 2.f);

		static sf::RectangleShape underline;
		underline.setSize(sf::Vector2f(15, 5));
		sf::Vector2f bounds = initials.getGlobalBounds().getSize();
		switch (namePos)
		{
		case 0:
			underline.setPosition(viewport->getCenter() + sf::Vector2f(-25 - 5, 30));
			break;

		case 1:
			underline.setPosition(viewport->getCenter() + sf::Vector2f(0 - 5, 30));
			break;

		case 2:
			underline.setPosition(viewport->getCenter() + sf::Vector2f(25 - 5, 30));
			break;
		}

		// TODO add timer for death animation
		if (playerState.lives > 0) 
			target.draw(entityManager, states);
		else
		{
			target.draw(initials, states);
			target.draw(underline, states);
		}
	}

	static void setView(sf::RenderWindow& window, sf::View& viewport_)
	{
		// if change size reassign window.setView
		window.setView(viewport_);
		viewport = &viewport_;
	}
private:
	static bool SaveHighscore(Action& actions);


	static EntityManager entityManager;
	static sf::View* viewport;
	static Timer<double> hyperspaceCooldown;
	static PlayerState playerState;
	static char name[3];
	static uint8_t namePos;

	static const char validChars[];
};
