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
		    {0,18,15,6}, // Sprite IntRect
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
			{48, 0, 3, 8},
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
			{13,9,3,3},
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
			{9,9,8,8},
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
			{23,9,9,8},
			4
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
			{63,9,11,4},
			2
		},
		Vec2<double>{
			2.0f,
			3.0f
		},
		200
	},
	// Bomber
	EntityData{
		SpriteData{
			{52,0,6,7},
			4
		},
		Vec2<double>{
			0,
			0
		},
		250
	},
	// Pod
	EntityData{
		SpriteData{
			{40,0,7,7},
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
			{17,9,5,4},
			1
		},
		Vec2<double>{
			1.0f / 3.0f * 2,
			1.0f / 2.0f * 2
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
			1.0f,
			1.5f,
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

	//animation.printDebugInfo();
	wrap(); // Overridden in player

	animation.tick(deltatime);
	animation.setPosition(pos);
	//std::cout << '\n';
}

bool Entity::collide(Entity* other)
{
	return animation.intersects(other->animation);
}

void Entity::wrap()
{
	if (pos.x < 0)
		pos.x += COMN::worldSize;
	else if (pos.x > COMN::worldSize)
		pos.x -= COMN::worldSize;
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

const Entity::EntityTarget Entity::makePlayerTargetedVec(sf::Vector2f pos, EntityID::ID ID, uint8_t scale = 1)
{
	static constexpr float half = COMN::worldSize / 2;

	sf::Vector2f target = makeCenteredTL({ 0, 0 }, EntityID::PLAYER);
	sf::Vector2f eVel = getEVel(ID);

	pos = makeCenteredTL(pos, ID);
	// Use the center & normalize
	pos.x -= - half;

	// Normalize the target
	target.x += EntityData::PLAYER_REF.pos->x - pos.x - half;
	target.y += EntityData::PLAYER_REF.pos->y - pos.y;

	// Apply wrapping
	if (target.x > half)
		target.x -= COMN::worldSize;
	else if (target.x < -half)
		target.x += COMN::worldSize;

	// Generate the angle
	double rot = atan2(target.y, target.x);

	// Generate the velocity
	sf::Vector2f vel = {
		(float)(cos(rot) * scale * eVel.x),
		(float)(sin(rot) * scale * eVel.y)
	};
	vel += *EntityData::PLAYER_REF.vel;


	return { vel, target };
}

const sf::Vector2f Entity::makeCenteredTL(sf::Vector2f pos, EntityID::ID id)
{
	return {
		pos.x + (float)(DATA_TABLE[id].SPRITE_DATA.bounds.width / 2.),
		pos.y + (float)(DATA_TABLE[id].SPRITE_DATA.bounds.height  / 2.)
	};
}

bool Entity::isOnScreen()
{
	static constexpr float half = COMN::resolution.x / 2;
	float left  = DisplayManager::getView().getCenter().x - half,
		  right = DisplayManager::getView().getCenter().x + half;
	return (pos.y >= 0 && pos.y <= COMN::resolution.y) &&
		(
			(pos.x >= left - COMN::worldSize && pos.x <= right - COMN::worldSize) ||
			(pos.x >= left && pos.x <= right) ||
			(pos.x >= left + COMN::worldSize && pos.x <= right + COMN::worldSize)
			);
}
