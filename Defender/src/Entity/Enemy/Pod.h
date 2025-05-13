#pragma once
#include "../Enemy.h"
class Pod :
    public Enemy
{
public:
    explicit Pod(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr);

    virtual ~Pod() override;

    virtual void tick(double deltatime) override;

private: 
    int16_t rot;
};

