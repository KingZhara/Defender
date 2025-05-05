#pragma once
#define _USE_MATH_DEFINES
#include <math.h>   // for M_PI
#include <new>
#include <random>

#include <SFML/Graphics.hpp>
#include "../Entity/Entity.h"

class DeathAnimation : public sf::Drawable
{
public:
    DeathAnimation(sf::Vector2f pos)
    {
		pieces = (Entity*)malloc(500 * sizeof(Entity));

        for (int i = 0; i < 500; i++)
        {
			static std::mt19937 rng(std::random_device{}()); // seed once globally or per context

			// Uniform speed in range [minSpeed, maxSpeed]
			std::uniform_real_distribution<float> speedDist(10.f, 100.f);

			// Uniform direction in [0, 2?)
			std::uniform_real_distribution<float> angleDist(0.f, 2.f * static_cast<float>(M_PI));

			// Create entity
			Entity* piece = &pieces[i];

			float speed = speedDist(rng);
			float angle = angleDist(rng);

			new (piece) Entity(pos, EntityID::DEATH_ANIM_PIECE, sf::IntRect{ 92, 0, 2, 2 });

			piece->setVel({ std::cos(angle) * speed, std::sin(angle) * speed });
        }
    }

	~DeathAnimation()
	{
		for (int i = 0; i < 100; i++)
		{
			pieces[i].~Entity();
		}
		free(pieces);
	}

    void tick(double deltatime);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
	Entity* pieces;
};

