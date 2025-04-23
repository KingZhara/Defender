#include "../Game.h"

#include <iostream>

Game::Game()
{
}

Game::~Game()
{
}

void Game::tick(Action& actions, double deltatime)
{
	//std::cout << "Deltatime: " << deltatime << '\n';
	//std::cout << "Viewport Data: " << DisplayManager::getView().getSize().x << ", " << DisplayManager::getView().getSize().y << " | " << DisplayManager::getView().getCenter().x << DisplayManager::getView().getCenter().y << '\n';

	if (state.type != GameState::State::STAGE && actions)
		state.switchState(true);

	if (state.tick(actions, deltatime))
		state.switchState();

	// Placed here solely for deltatime access
	UserInterface::shaderTick(deltatime);
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	state.draw(target, states);
}
