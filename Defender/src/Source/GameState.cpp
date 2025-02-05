#include "../GameState.h"

// Load default state
GameState::GameState()
{
	state = State::ATTRACT;
}

// TODO free state memory
GameState::~GameState()
{
}

void GameState::tick()
{
	switch (state)
	{
	case State::ATTRACT: attractState.tick(); break;
	case State::HIGHSCORE: highscoreState.tick(); break;
	case State::STAGE: stageState.tick(); break;
	}
}

void GameState::draw(const sf::RenderWindow& window)
{
	switch (state)
	{
	case State::ATTRACT: attractState.draw(window); break;
	case State::HIGHSCORE: highscoreState.draw(window); break;
	case State::STAGE: stageState.draw(window); break;
	}
}

void GameState::switchState(State newState)
{
	if (newState != state)
	{
		switch (state)
		{
		case State::ATTRACT: attractState.~AttractState(); break;
		case State::HIGHSCORE: highscoreState.~HighscoreState(); break;
		case State::STAGE: stageState.~StageState(); break;
		}
		
		state = newState;
	}
}
