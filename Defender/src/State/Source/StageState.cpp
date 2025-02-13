#include "../StageState.h"
#include <iostream>

StageState::StageState()
{
}

StageState::~StageState()
{
}

bool StageState::tick(Action& actions)
{
	std::cout << "STAGE\n";
	return false;
}