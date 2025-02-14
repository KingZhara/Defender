#include "../GameState.h"

#include <iostream>

// Load default state
GameState::GameState()
{
	type = State::ATTRACT;
	new (&state) AttractState();
}

// TODO free state memory
GameState::~GameState()
{
	// Destroy the appropriate state
	switch (type)
	{
	case State::ATTRACT:   std::get<AttractState>(state).~AttractState();     break;
	case State::HIGHSCORE: std::get<HighscoreState>(state).~HighscoreState(); break;
	case State::STAGE:     std::get<StageState>(state).~StageState();         break;
	 }
}

bool GameState::tick(Action& actions)
{
	// Map to the appropriate tick method
	switch (type)
	{
	case State::ATTRACT:   return std::get<AttractState>(state).tick();
	case State::HIGHSCORE: return std::get<HighscoreState>(state).tick();
	case State::STAGE:     return std::get<StageState>(state).tick(actions);
	default:
		return true;
	}
}

void GameState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	std::cout << "Dra2\n";
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
	// Destroy the current state before switching
	this->~GameState();

	// Set the new state type
	if (stage)
	{
		type = State::STAGE;
		state.emplace<StageState>(StageState());
	}
	else
	{
		type = (type == State::ATTRACT) ? State::HIGHSCORE :
			(type == State::HIGHSCORE) ? State::ATTRACT :
			State::HIGHSCORE;

		if (type == State::ATTRACT)
			state.emplace<AttractState>(AttractState());
		else
			state.emplace<HighscoreState>(HighscoreState());
	}
}
