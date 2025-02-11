#pragma once

/*
	Game runs most of the program
	Game holds a GameState, which holds the actual game,
	and the attract and highscore screens.

	GameState switches the state screens and updates them

	StageState updates the actual game
*/

#include <SFML/Graphics.hpp>

#include "GameState.h"
#include "Utility/Action.h"

class Game : public sf::Drawable
{
public:

	Game();

	~Game();

	void tick(Action& actions);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:

	GameState state;
};

