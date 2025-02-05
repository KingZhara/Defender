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

class Game
{
public:

	Game();

	~Game();

	void tick();

	void draw(const sf::RenderWindow& window);

private:

	GameState state;
};

