#include "../Entity.h"


const Entity::SpriteData Entity::SpriteTable[EntityID::LENGTH] =
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