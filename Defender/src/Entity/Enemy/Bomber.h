#pragma once

#include "../Enemy.h"

class Bomber : public Enemy
{
public:
	explicit Bomber(sf::Vector2f pos_,
		bool         isScripted_ = false, EntityScript* script_ = nullptr);

	virtual ~Bomber() override = default;

    virtual void tick(double deltaTime) override;

private:
	Timer<double> attackTimer{ 0.6 };
    uint8_t yMovTimStart = (rand()%20)+10;
	uint8_t range = (rand() % 20) + 5;

	uint8_t yMovTim = 20;
	uint8_t stopTim = 20;

	int16_t dx = 0;

	uint8_t rerollTim = 0;
};

