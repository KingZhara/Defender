#pragma once
#include "../Enemy.h"
class Lander :
    public Enemy
{
public:
    explicit Lander(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr);

    virtual ~Lander() override = default;

    virtual void tick(double deltaTime) override;
    bool         hasTarget();
    bool         shouldMutate();
    void         setTarget(Entity* target);

private:
	Entity* target = nullptr;
    Timer<double> wanderTimer{ 5, false };
    Timer<double>      attackTimer{ 3 }; // @todo verify time
    bool dir = false, holding = false;
};

