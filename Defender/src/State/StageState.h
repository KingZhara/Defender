#pragma once

#include <SFML/Graphics.hpp>
#include "../Utility/Action.h"
#include "../Utility/EntityManager.h"

class StageState : public sf::Drawable
{
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

	EntityManager entityManager = EntityManager(false);
	static sf::View* viewport;
};
