#pragma once

#include <SFML/Graphics.hpp>

#include "State/State.hpp"

class GameState
{
public:

	enum class State
	{
		ATTRACT,
		HIGHSCORE,
		STAGE
	};

	GameState();

	~GameState();

	void tick();

	void draw(const sf::RenderWindow& window);

	void switchState(State newState);

private:

	State state;

	union
	{
		AttractState attractState;
		HighscoreState highscoreState;
		StageState stageState;
	};

};

