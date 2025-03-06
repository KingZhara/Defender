#pragma once

#include <SFML/Graphics.hpp>
#include "../Utility/Action.h"
#include "../Utility/EntityManager.h"

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
		target.draw(entityManager, states);
	}

	static void setView(sf::RenderWindow& window, sf::View& viewport_)
	{
		// if change size reassign window.setView
		window.setView(viewport_);
	}
private:
	static bool SaveHighscore(Action& actions);


	static EntityManager entityManager;
	static sf::View* viewport;
	static Timer<double> hyperspaceCooldown;
	static PlayerState playerState;
};
