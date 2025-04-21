#include "../Entity.h"

#include "../../Utility/DisplayManager.h"

std::queue<Entity::QueuedEntity> Entity::entityQueue;

sf::Vector2f* Entity::EntityData::PlayerRef::pos = nullptr;
sf::Vector2f* Entity::EntityData::PlayerRef::vel = nullptr;

const Entity::EntityData Entity::DATA_TABLE[EntityID::LENGTH] =
{
	// Player
	EntityData{
		// Sprite Data
		SpriteData{
		    {0,16,15,6}, // Sprite IntRect
			2, // Sprite frame count
			// Shader type
			// Custom frame length
		},
		// Velocity factor
		Vec2<double>{
			1.5f,
			1.0f
		},
	    // XP
		0
	},
	// Astronaut
	EntityData{
	    SpriteData{
			{43, 0, 3, 8},
			1
		},
		Vec2<double>{
			1.0f,
			1.0f
		},
		250
	},
	// Bullet
	EntityData{
		SpriteData{
			{8,8,3,3},
			1
		},
		Vec2<double>{
			1.0f,
			1.0f
		},
		0
	},
	// Laser
    // @todo Replace later; blank data, will not use a sprite for drawing, custom image
	EntityData{
		SpriteData{
			{19,0,8,8},
			2
		},
		Vec2<double>{
			1.0f,
			1.0f
		},
		0
	},
	// Bomb
	EntityData{
		SpriteData{
			{19,0,8,8},
			2
		},
		Vec2<double>{
			1.0f,
			1.0f
		},
		0
	},
	// Lander
	EntityData{
		SpriteData{
			{19,8,9,8},
			3
		},
		Vec2<double>{
			1.0f,
			1.0f
		},
		150
	},
	// Mutant
	EntityData{
		SpriteData{
			{0,0,10,8},
			1,
			ShaderID::RAND_COL
		},
		Vec2<double>{
			1.0f,
			1.0f
		},
		150
	},
	// Baiter
	EntityData{
		SpriteData{
			{55,8,11,4},
			2
		},
		Vec2<double>{
			1.0f,
			1.0f
		},
		200
	},
	// Bomber
	EntityData{
		SpriteData{
			{45,0,6,7},
			3
		},
		Vec2<double>{
			1.0f,
			1.0f
		},
		250
	},
	// Pod
	EntityData{
		SpriteData{
			{36,0,7,7},
			1
		},
		Vec2<double>{
			1.0f,
			1.0f
		},
		1000
	},
	// Swarmer
	EntityData{
		SpriteData{
			{14,8,5,4},
			1
		},
		Vec2<double>{
			1.0f,
			1.0f
		},
		150
	},
	// Particle
	EntityData{
		SpriteData{
			{0,0,1,0},
			1
		},
		Vec2<double>{
			0.001f,
			0.001f,
		},
		0

	},
};

void Entity::tick(double deltatime)
{
	pos += {(float)(vel.x * deltatime), (float)(vel.y* deltatime)};

	if (abs(vel.x) < 0.1f)
		vel.x = 0;
	if (abs(vel.y) < 0.1f)
		vel.y = 0;


	animation.tick(deltatime);
	animation.setPosition(pos);
	//std::cout << '\n';
}

bool Entity::collide(Entity* other)
{
	return animation.intersects(other->animation);
}

sf::IntRect const & Entity::getBounds(const EntityID::ID ID)
{
	return DATA_TABLE[ID].SPRITE_DATA.bounds;
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

const sf::Vector2f Entity::makePlayerTargetedVec(sf::Vector2f pos, uint8_t scale)
{
	double                rot = atan2(EntityData::PLAYER_REF.pos->y - pos.y, EntityData::PLAYER_REF.pos->x - pos.x);
	return{
		(float)cos(rot) * scale + EntityData::PLAYER_REF.vel->x,
		(float)sin(rot) * scale + EntityData::PLAYER_REF.vel->y
	};
}
