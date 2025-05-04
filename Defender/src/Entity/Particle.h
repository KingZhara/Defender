#pragma once
#include "Entity.h"
#include "Player.h"


class Particle : public Entity
{
public:
    Particle(sf::Vector2f        pos,
             EntityID::ID        ID_,
             bool                spawning,
             sf::Vector2<int8_t> collision,
             Entity *            entity_);

	virtual ~Particle() override
	{
		free(pieces);
		pieces = nullptr;

    virtual void               tick(double deltatime) override;
    virtual void               draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    bool                       isComplete() const;
    static sf::Vector2<int8_t> defCent();
    Entity *                   getEntity();

private:
    static constexpr uint8_t PARTICLE_SIZE = 2;
    sf::Vector2<uint8_t>     size;
    Timer<double>            lifetime{1, false};
    Entity *                 pieces     = nullptr;
    bool                     skipCenter = false;
    Entity *                 entity;
};
