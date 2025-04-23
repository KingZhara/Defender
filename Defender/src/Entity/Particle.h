#pragma once
#include "Entity.h"


class Particle : public Entity
{
public:
	Particle(sf::Vector2f pos, EntityID::ID ID_, bool spawning, sf::Vector2<int8_t> collision, Entity* entity_);

	virtual ~Particle() override
	{
		_aligned_free(pieces);
		pieces = nullptr;
		delete entity;
	};

	virtual void tick(double deltatime) override;
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	bool isComplete() const { return lifetime.isComplete(); }
	static sf::Vector2<int8_t> defCent()
	{
		return { -1, -1 };
	}
	Entity* getEntity()
	{
		Entity* tmp = entity;
		entity = nullptr;

		return tmp;
	}

private:
	static constexpr uint8_t PARTICLE_SIZE = 2;
	sf::Vector2<uint8_t> size;
	Timer<double> lifetime{ 1, false};
	Entity* pieces = nullptr;
	bool skipCenter = false;
	Entity* entity;

};

