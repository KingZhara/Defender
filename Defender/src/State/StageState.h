#pragma once

#include <SFML/Graphics.hpp>
#include "../Utility/Action.h"
#include "../Utility/EntityManager.h"

class StageState : public sf::Drawable
{
public:
	StageState();

	~StageState() = default;

	bool tick(Action& actions);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(entityManager, states);
	}

	static void setView(sf::RenderWindow& window)
	{
		// if change size reassign window.setView
		window.setView(viewport);
	}

	EntityManager entityManager = EntityManager(false);
	static sf::View viewport;
};
