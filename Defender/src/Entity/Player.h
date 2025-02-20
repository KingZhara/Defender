#pragma once
#include "Entity.h"
#include "../Utility/Action.h"

class Player :
    public Entity
{
public:
    void setActions(Action& actions);
private:
    Action actions;

};
