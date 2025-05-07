#include "../Particle.h"
#include <algorithm>
#include <new>

#include "../Entity.h"
#include <cstdlib>

#include "../Entity.h"

Particle::Particle(sf::Vector2f pos, EntityID::ID ID_, bool spawning, sf::Vector2<int8_t> collision, Entity* entity_) : Entity(pos, EntityID::PARTICLE, sf::IntRect{}), entity(entity_)
{
	const sf::IntRect bounds = VisualComponent::getBounds(ID_);
	sf::Vector2f baseVel = getEVel(ID);
	// Make sure we round up
	size = {
		static_cast<uint8_t>((bounds.width  + PARTICLE_SIZE - 1) / PARTICLE_SIZE),
		static_cast<uint8_t>((bounds.height + PARTICLE_SIZE - 1) / PARTICLE_SIZE) };

	if (std::numeric_limits<int8_t>::max() < (bounds.width  + PARTICLE_SIZE - 1) / PARTICLE_SIZE ||
		std::numeric_limits<int8_t>::max() < (bounds.height + PARTICLE_SIZE - 1) / PARTICLE_SIZE)
	{
		throw std::overflow_error("Privided Glyph is too large for this type");
	}
	// If collision = {-1, -1}, reset
    if (collision.x == -1 || collision.y == -1)
	{
		collision.x = (size.x + 1) / 2;
	    collision.y = (size.y + 1) / 2;
	
		//skipCenter = true;
	}
	// Create the array pointer, size.x * size.y - 1 (collision), if spawning
	pieces = static_cast<Entity*>(malloc(sizeof(Entity) * (size.x * size.y - (skipCenter ? 1 : 0))));
	if (!pieces) throw std::bad_alloc();

    collision.x = std::min<int>(collision.x, size.x);
    collision.y = std::min<int>(collision.y, size.y);
    collision.x = std::max<int>(collision.x, 0);
    collision.y = std::max<int>(collision.y, 0);

    //std::cout << "Size: " << (size.x * size.y - (skipCenter ? 1 : 0)) << '\n';

	// For each piece, create a new particle
    /* // For x; +=2
     *
     * /* // For y; +=2
        *
        * Grid Pos = {x / PARTICLE_SIZE, y / PARTICLE_SIZE} - collision
        *
        * // Pos is (pos + {x, y})
        * // Vel is Grid Pos
        *
        * Bounds Pos = ((this bounds pos) + {x, y})
		* Bounds size = min(PARTICLE_SIZE, bounds_size - Grid Pos)
        /
     *
     */

	uint16_t index = 0;

	for (int8_t y = 0; y < bounds.height; y += PARTICLE_SIZE)
	{
		for (int8_t x = 0; x < bounds.width; x += PARTICLE_SIZE)
		{
			// Get the grid position based on the collision as the origin
			sf::Vector2<int8_t> gridPos = { (int8_t)(x / PARTICLE_SIZE), (int8_t)(y / PARTICLE_SIZE) };

			gridPos -= collision;

			// Skip the collision piece; Only triggered when collision piece is within bounds
			//if (gridPos.x == 0 && gridPos.y == 0)
			//	continue;

			// Get the real position of the piece
			sf::Vector2f nPos = pos;
			nPos += {(float)x, (float)y};

			// Velocity is the grid pos
			sf::Vector2f nVel = {
				(float)(gridPos.x),
				(float)(gridPos.y)
			};

			nVel.x *= baseVel.x;
			nVel.y *= baseVel.y;

			// Modify position if spawning
			if (spawning)
			{
				// Velocity is based around the collision;
				// the offset is based around the velocity as an initial scale,
				// then the lifetime in seconds as a multiplier for length,
				// and lastly the speed factor for pixel accuracy.
				nPos.x += (float)(nVel.x * lifetime.getBase());// +gridPos.x;
				nPos.y += (float)(nVel.y * lifetime.getBase());// +gridPos.y;

				// Flip the velocity of the pieces to move inwards
				nVel = -nVel;
			}

			// The texture bounds of this particle
			sf::IntRect nBounds = {
				sf::Vector2i{ // Pos
					bounds.getPosition().x + x,
					bounds.getPosition().y + y
				},
				sf::Vector2i{ // Size
					std::min<uint8_t>(PARTICLE_SIZE, std::max(0, (int)(pos.x + x - bounds.getSize().x))),
					std::min<uint8_t>(PARTICLE_SIZE, std::max(0, (int)(pos.y + y - bounds.getSize().y))),
				}
			};


			// Create the piece
			new (&pieces[index]) Entity(nPos, EntityID::PIECE,nBounds);
			pieces[index].setVel(nVel);


			// Increment the index of the piece we are adding
			++index;
		}
	}

	lifetime.tick(0);
	if (lifetime.isComplete())
		throw std::runtime_error("Particle issue...");
	//if (spawning)
	//	lifetime.tick(29);
}

Particle::~Particle() {
    free(pieces);
    pieces = nullptr;

    if (dynamic_cast<Player*>(entity))
        std::cout << "Destroyed from particle\n";

    delete entity;
}

void Particle::tick(double deltatime)
{
	//std::cout << "Tick Particle\n";
	if (!lifetime.isComplete())
	{
		for (int8_t i = 0; i < size.x * size.y - (skipCenter ? 1 : 0); i++)
		    pieces[i].tick(deltatime);

		lifetime.tick(deltatime);
	}
}

void Particle::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	// Draw all pieces
	if (!lifetime.isComplete())
		for (int8_t i = 0; i < size.x * size.y - (skipCenter ? 1 : 0); i++)
			target.draw(pieces[i], states);

	// Set mini data
	if (entity)
	{
		if (entity->getID() >= EntityID::LANDER && entity->getID() <= EntityID::SWARMER)
			miniSprite->setTextureRect(sf::IntRect(80 + (entity->getID() - EntityID::LANDER) * 4, 0, 2, 2));
		else if (entity->getID() == EntityID::PLAYER)
			miniSprite->setTextureRect(sf::IntRect(80, 3, 3, 3));
		else if (entity->getID() == EntityID::ASTRONAUT)
			miniSprite->setTextureRect(sf::IntRect(84, 3, 2, 2));
		else throw std::runtime_error("Particle: Entity ID not found");

		Entity::draw(target, states);
	}
}

bool Particle::isComplete() const { return lifetime.isComplete(); }
sf::Vector2<int8_t> Particle::defCent() {
    return { -1, -1 };
}

Entity * Particle::getEntity() {
    Entity* tmp = entity;
    entity      = nullptr;

    return tmp;
}
