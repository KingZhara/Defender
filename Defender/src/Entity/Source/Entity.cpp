#include "../Entity.h"


const Entity::SpriteData Entity::SPRITE_TABLE[EntityID::LENGTH] =
{
	{ // Player
		{}, // Sprite IntRect
		{}, // Sprite frame count
		ShaderID::NONE, // Sprite shader (Optional parameter)
		1./15., // Sprite frame length (Optional parameter, double)
	},
	{ // Astronaut
	},
	{ // Bullet
	},
	{ // Laser
	},
	{ // Bomb
	},
	{ // Lander
	},
	{ // Mutant
	},
	{ // Baiter
	},
	{ // Bomber
	},
	{ // Pod
	},
	{ // Swarmer
	}
};

void Entity::tick()
{
}

bool Entity::collide(Entity* other)
{
	return animation.getBounds().intersects(other->animation.getBounds());
}

void Entity::setPos(sf::Vector2f newPos)
{
	pos = newPos;
}


const uint16_t Entity::XP_TABLE[EntityID::LENGTH] =
{
	0,    // PLAYER
	250,  // ASTRONAUT
	0,    // BULLET
	0,    // LASER
	0,    // BOMB
	150,  // LANDER
	150,  // MUTANT
	200,  // BAITER
	250,  // BOMBER
	1000, // POD
	150   // SWARMER
};