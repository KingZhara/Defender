#pragma once
#include "../Enemy.h"

#include "../../Utility/UserInterface/UserInterface.h"

class Mutant :
    public Enemy
{
public:
    explicit Mutant(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr);

    virtual ~Mutant() override = default;

    virtual void tick(double deltatime) override;

private:
    // Range [0, 3]; 0, 1 = up; 2, 3 = down;
    uint8_t bobStage = 0;
    Timer<double> randomFactor{ 3 };
    Timer<double> attackTimer{ 2 };
};

