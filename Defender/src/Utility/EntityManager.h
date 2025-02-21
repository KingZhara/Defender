#pragma once
#include <vector>
#include <concepts>
#include "Action.h"
#include "../Entity/Entity.hpp"



class EntityManager : public sf::Drawable
{
	template<typename T>
	struct EntityHolder
	{
		std::vector<T*> entities;

		// Dead entity search information
		uint16_t first, last, count;

		void updateData(bool death, uint16_t index)
		{
			// Update the death counter
			death ? ++count
				  : --count;

			// Update domain indices
			if (index < first)
				first = index;
			else if (index > last)
				last = index;
		}
	};

public:
	enum class SpawnType
	{
		PLAYER,
		PROJECTILE,
		ENEMY,
		ASTRONAUT,
	};

	EntityManager(bool scripted);

	~EntityManager();

	
	bool tick(Action& actions);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void particleize(bool spawn, sf::Vector2f pos, EntityID::EntityID ID);
	void killArea(sf::Vector2<uint16_t> domain);

	// @todo If time permits, play with optimization, potentially using a spacial tree.
	template<typename T>
	void collisionWrapper(uint16_t projectile, EntityHolder<T>& entities) {
		for (uint16_t i = 0; i < entities.entities.size(); i++)
		{
			if (entities.entities.at(i) != nullptr &&
				entities.entities.at(i)->collide(projectiles.entities.at(projectile)))
			{
				particleize(false, entities.entities.at(i)->getPos(), entities.entities.at(i)->getID());

				delete projectiles.entities.at(projectile);
				delete entities.entities.at(i);

				projectiles.entities.at(projectile) = nullptr;
				entities.entities.at(i) = nullptr;

				projectiles.updateData(true, projectile);
				entities.updateData(true, i);
			}
		}
	}

private:
	EntityHolder<Projectile> projectiles;
	EntityHolder<Enemy>      enemies;
	EntityHolder<Astronaut>  astronauts;
	EntityHolder<Entity>     particles; // Always scripted
	Player* player = nullptr;

	bool scripted;
};

