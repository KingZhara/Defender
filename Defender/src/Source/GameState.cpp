#include "../GameState.h"

// Load default state
GameState::GameState()
{
	type = State::ATTRACT;
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
	// Map to the correct state switching logic
	switch (type)
	{
	case State::ATTRACT:
		attractState.~AttractState();
		if (stage)
			stageState     = StageState();
		else
			highscoreState = HighscoreState();
		break;

	case State::HIGHSCORE:
		highscoreState.~HighscoreState();
		if (stage)
			stageState   = StageState();
		else
			attractState = AttractState();
		break;

	case State::STAGE:
		stageState.~StageState();
		highscoreState = HighscoreState();
		break;
	}
}
