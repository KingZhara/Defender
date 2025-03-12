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
		initials.setFont(UserInterface::getFont());
		initials.setString(name);
		initials.setPosition(viewport->getCenter());

		// TODO add timer for death animation
		if (playerState.lives > 0) 
			target.draw(entityManager, states);
		else
			target.draw(initials);
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
};
