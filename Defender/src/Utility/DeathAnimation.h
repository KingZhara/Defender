#pragma once
#define _USE_MATH_DEFINES
#include <math.h>   // for M_PI
#include <new>
#include <SFML/Graphics.hpp>
#include "../Entity/Entity.h"

class DeathAnimation : public sf::Drawable
{
public:
    DeathAnimation(sf::Vector2f pos)
    {
		pieces = (Entity*)malloc(100 * sizeof(Entity));

        for (int i = 0; i < 100; i++)
        {
            Entity* piece = &pieces[i];

            float vel = rand() % 100 + 1;
            float rot = static_cast<float>(rand()) / RAND_MAX * 2.0f * M_PI;

			new (piece) Entity(pos, EntityID::DEATH_ANIM_PIECE, sf::IntRect{ 92, 0, 2, 2 });

            piece->setVel({ std::cos(rot), std::sin(rot) });
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

