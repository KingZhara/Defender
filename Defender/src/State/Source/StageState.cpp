#include "../StageState.h"
#include <iostream>

StageState::StageState()
{
}

bool StageState::tick(Action& actions)
{
	if (actions.flags.hyperspace)
		entityManager.hyperspace(viewport.getViewport());
	if (actions.flags.smart_bomb)
		entityManager.killArea(viewport.getViewport());

	std::cout << "STAGE\n";
	return false;
}