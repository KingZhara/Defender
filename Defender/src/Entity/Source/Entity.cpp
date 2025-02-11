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


const uint16_t Entity::XP_TABLE[EntityID::LENGTH] =
{
	0,
	250,
	0,
	0,
	0,
	150,
	150,
	200,
	250,
	1000,
	150
};