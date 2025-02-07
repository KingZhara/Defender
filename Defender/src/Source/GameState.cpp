#include "../GameState.h"

// Load default state
GameState::GameState()
{
	type = State::ATTRACT;
	new (&attractState) AttractState();
}

// TODO free state memory
GameState::~GameState()
{
	// Destroy the appropriate state
	switch (type)
	{
	case State::ATTRACT:   attractState.~AttractState();     break;
	case State::HIGHSCORE: highscoreState.~HighscoreState(); break;
	case State::STAGE:     stageState.~StageState();         break;
	 }
}

bool GameState::tick(Action& actions)
{
	// Map to the appropriate tick method
	switch (type)
	{
	case State::ATTRACT:   return attractState.tick();
	case State::HIGHSCORE: return highscoreState.tick();
	case State::STAGE:     return stageState.tick(actions);
	default:
		return true;
	}
}

void GameState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Map to the appropriate draw method
	switch (type)
	{
	case State::ATTRACT:   attractState.draw(target, states);   break;
	case State::HIGHSCORE: highscoreState.draw(target, states); break;
	case State::STAGE:     stageState.draw(target, states);     break;
	}
}

void GameState::switchState(bool stage)
{
	// Destroy the current state before switching
	this->~GameState();

	// Set the new state type
	if (stage)
	{
		type = State::STAGE;
		new (&stageState) StageState();
	}
	else
	{
		type = (type == State::ATTRACT) ? State::HIGHSCORE :
			(type == State::HIGHSCORE) ? State::ATTRACT :
			State::HIGHSCORE;

		if (type == State::ATTRACT)
			new (&attractState) AttractState();
		else
			new (&highscoreState) HighscoreState();
	}
}
