#include "../Game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::tick()
{
	state.tick();
}

void Game::draw(const sf::RenderWindow& window)
{
	state.draw(window);
}
