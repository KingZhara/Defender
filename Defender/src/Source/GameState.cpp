#include "../GameState.h"

#include <iostream>

// Load default state
GameState::GameState()
{
	type = State::ATTRACT;
	new (&state) HighscoreState();
}

bool GameState::tick(Action& actions, double deltatime)
{
	// Map to the appropriate tick method
	switch (type)
	{
	case State::ATTRACT:   return std::get<AttractState>(state).tick(deltatime);
	case State::HIGHSCORE: return std::get<HighscoreState>(state).tick(deltatime);
	case State::STAGE:     return std::get<StageState>(state).tick(actions, deltatime);
	case State::SCANNER:   return std::get<ScannerState>(state).tick(deltatime);
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
	case State::SCANNER:   std::get<ScannerState>(state).draw(target, states);   break;
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
		std::cout << "SWTCH: " << (uint16_t)type << '\n';

		switch (type)
		{
		case State::HIGHSCORE:
			type = State::SCANNER;
			state.emplace<ScannerState>();
			break;

		case State::SCANNER:
			type = State::ATTRACT;
			state.emplace<AttractState>();
			break;

		default:
			type = State::HIGHSCORE;
			state.emplace<HighscoreState>();
			break;
		}
	}
}