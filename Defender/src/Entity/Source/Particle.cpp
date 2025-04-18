#include "../Particle.h"
#include <new>

Particle::Particle(sf::Vector2f pos, EntityID::ID ID_, bool spawning, sf::Vector2<int8_t> center) : Entity(pos)
{
	// Make sure we round up
	size = {
		static_cast<uint8_t>((DATA_TABLE[ID_].SPRITE_DATA.bounds.width  + PARTICLE_SIZE - 1) / PARTICLE_SIZE),
		static_cast<uint8_t>((DATA_TABLE[ID_].SPRITE_DATA.bounds.height + PARTICLE_SIZE - 1) / PARTICLE_SIZE) };

	if (std::numeric_limits<int8_t>::max() < (DATA_TABLE[ID_].SPRITE_DATA.bounds.width  + PARTICLE_SIZE - 1) / PARTICLE_SIZE ||
		std::numeric_limits<int8_t>::max() < (DATA_TABLE[ID_].SPRITE_DATA.bounds.height + PARTICLE_SIZE - 1) / PARTICLE_SIZE)
	{
		throw std::overflow_error("Privided Glyph is too large for this type");
	}

	// If center = {-1, -1}, reset 

	// Create the array pointer, size.x * size.y - 1 (center), if spawning

	// For each piece, create a new particle
    /* // For x; +=2
     *
     * /* // For y; +=2
        *
        * Grid Pos = {x / PARTICLE_SIZE, y / PARTICLE_SIZE} - center
        *
        * // Pos is (pos + {x, y})
        * // Vel is Grid Pos
        *
        * Bounds Pos = ((this bounds pos) + {x, y})
		* Bounds size = min(PARTICLE_SIZE, bounds_size - Grid Pos)
        /
     *
     */

	// @todo include class board ocntent on spawn = true


}

void Particle::tick(double deltatime)
{
	if (!lifetime.isComplete())
	{
		for (int8_t x = 0; x < size.x; x++)
			for (int8_t y = 0; y < size.y; y++)
				pieces[x * size.y + y].tick(deltatime);

		lifetime.tick(deltatime);
	}
}

void Particle::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	// Draw all pieces
	if (!lifetime.isComplete())
    	for (int8_t x = 0; x < size.x; x++)
	    	for (int8_t y = 0; y < size.y; y++)
    			target.draw(pieces[x * size.y + y], states);
}
