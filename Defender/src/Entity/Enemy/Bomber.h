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
private:

	void tick(double deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

