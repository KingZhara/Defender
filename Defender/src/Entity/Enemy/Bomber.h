#pragma once

#include "../Enemy.h"

class Bomber : public Enemy
{
public:
	explicit Bomber(sf::Vector2f pos_,
		bool         isScripted_ = false, EntityScript* script_ = nullptr)
		: Enemy(pos_, EntityID::BOMBER, isScripted_, script_)
	{}

	/*
	The bomber gives the player 250 xp upon death. Bombers seem to just fly 
	left or right with a little random variation in their height and drop 
	stationary bombs.
	*/
	void tick(double deltaTime);

	Timer<double> attackTimer{ 0.6 };
	uint8_t yMovTimStart = (rand()%20)+10;
	uint8_t range = (rand() % 20) + 5;

	uint8_t yMovTim = 20;
	uint8_t stopTim = 20;

	int16_t dx;

	uint8_t rerollTim = 0;
};

