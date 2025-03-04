#include "../StageState.h"
#include <iostream>

sf::View* StageState::viewport = nullptr;

StageState::StageState()
{
	entityManager.spawn(EntityManager::SpawnType::PLAYER, { 50, 50 }, EntityID::PLAYER);
	entityManager.spawn(EntityManager::SpawnType::ASTRONAUT, { 50, 50 }, EntityID::ASTRONAUT);
}

bool StageState::tick(Action& actions, double deltatime)
{
	if (actions.flags.hyperspace)
		entityManager.hyperspace(viewport->getViewport());
	if (actions.flags.smart_bomb)
		entityManager.killArea(viewport->getViewport());

	entityManager.tick(actions, deltatime);

	return false;
}