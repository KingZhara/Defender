#include "../Entity.h"

std::queue<Entity::QueuedEntity> Entity::entityQueue;

const Entity::SpriteData Entity::SPRITE_TABLE[EntityID::LENGTH] =
{
	{ // Player
		{0,16,15,6}, // Sprite IntRect
		2, // Sprite frame count
		// Shader type
		// Custom frame length
	},
	{ // Astronaut
		{43, 0, 3, 8},
		1
	},
	{ // Bullet
		{8,8,3,3},
		1
	},
	{ // Laser
		// @todo Replace later; blank data, will not use a sprite for drawing, custom image
		{19,0,8,8},
		2
	},
	{ // Bomb
		{19,0,8,8},
		2
	},
	{ // Lander
		{19,8,9,8},
		3
	},
	{ // Mutant
		{0,0,10,8},
		1
	},
	{ // Baiter
		{55,8,11,4},
		2
	},
	{ // Bomber
		{45,0,6,7},
		3
	},
	{ // Pod
		{35,0,7,7},
		1
	},
	{ // Swarmer
		{14,8,5,4},
		1
	}
};

sf::Vector2f* Entity::playerPos = nullptr;

void Entity::tick(double deltatime)
{
	animation.tick(deltatime);

	pos += vel;
	vel *= 0.9f;
	if (abs(vel.x) < 0.1f)
		vel.x = 0;
	if (abs(vel.y) < 0.1f)
		vel.y = 0;

	animation.setPosition(pos);
}

bool Entity::collide(Entity* other)
{
	return animation.getBounds().intersects(other->animation.getBounds());
}

void Entity::setPos(sf::Vector2f newPos)
{
	pos = newPos;
	animation.setPosition(newPos);
}

void Entity::setVel(sf::Vector2f newVel)
{

	vel = newVel;
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
