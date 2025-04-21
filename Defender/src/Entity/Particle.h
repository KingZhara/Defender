#pragma once
#include "Entity.h"

class Particle : public Entity
{
public:
	Particle(sf::Vector2f pos, EntityID::ID ID_, bool spawning = false, sf::Vector2<int8_t> collision = {-1, -1});

	virtual ~Particle() override
	{
		free(pieces);
		pieces = nullptr;
	};

	virtual void tick(double deltatime) override;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
	static constexpr uint8_t PARTICLE_SIZE = 2;
	sf::Vector2<uint8_t> size;
	Timer<double> lifetime{ 1, false};
	Entity* pieces = nullptr;


};

