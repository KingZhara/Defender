#include "../Game.h"

#include <iostream>

Game::Game()
{
}

Game::~Game()
{
}

void Game::tick(Action& actions)
{
	if (state.type != GameState::State::STAGE && actions)
		state.switchState(true);

	if (state.tick(actions))
		state.switchState();
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	state.draw(target, states);
}
