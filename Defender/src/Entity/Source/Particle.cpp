#include "../Particle.h"
#include <new>

Particle::Particle(sf::Vector2f pos, EntityID::ID ID_, bool spawning) : Entity(pos)
{
	// Make sure we round up
	size = {
		static_cast<uint8_t>((DATA_TABLE[ID_].SPRITE_DATA.bounds.width + PARTICLE_SIZE / 2) / PARTICLE_SIZE),
		static_cast<uint8_t>((DATA_TABLE[ID_].SPRITE_DATA.bounds.height + PARTICLE_SIZE / 2) / PARTICLE_SIZE) };

	if (std::numeric_limits<int8_t>::max() < (DATA_TABLE[ID_].SPRITE_DATA.bounds.width + PARTICLE_SIZE / 2) / PARTICLE_SIZE ||
		std::numeric_limits<int8_t>::max() < (DATA_TABLE[ID_].SPRITE_DATA.bounds.height + PARTICLE_SIZE / 2) / PARTICLE_SIZE)
	{
		throw std::overflow_error("Privided Glyph is too large for this type");
	}
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
