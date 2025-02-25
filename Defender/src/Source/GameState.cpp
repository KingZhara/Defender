#include "../GameState.h"

#include <iostream>

// Load default state
GameState::GameState()
{
	type = State::ATTRACT;
	new (&state) AttractState();
}

bool GameState::tick(Action& actions, double deltatime)
{
	// Map to the appropriate tick method
	switch (type)
	{
	case State::ATTRACT:   return std::get<AttractState>(state).tick(deltatime);
	case State::HIGHSCORE: return std::get<HighscoreState>(state).tick(deltatime);
	case State::STAGE:     return std::get<StageState>(state).tick(actions, deltatime);
	default:
		return true;
	}
}

void GameState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Map to the appropriate draw method
	switch (type)
	{
	case State::ATTRACT:   std::get<AttractState>(state).draw(target, states);   break;
	case State::HIGHSCORE: std::get<HighscoreState>(state).draw(target, states); break;
	case State::STAGE:     std::get<StageState>(state).draw(target, states);     break;
	}
}

void GameState::switchState(bool stage)
{
	// Set the new state type
	if (stage)
	{
		type = State::STAGE;
		state.emplace<StageState>(); // No need to construct then move
	}
	else
	{
		type = (type == State::ATTRACT) ? State::HIGHSCORE :
			(type == State::HIGHSCORE) ? State::ATTRACT :
			State::HIGHSCORE;

		if (type == State::ATTRACT)
			state.emplace<AttractState>();
		else
			state.emplace<HighscoreState>();
	}
}