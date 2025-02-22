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
		bool insertionSide = false;

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

	EntityManager(bool scripted_);

	~EntityManager() override = default;

	
	bool tick(Action& actions);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void particleize(bool spawn, sf::Vector2f pos, EntityID::EntityID ID);
	void killArea(sf::FloatRect viewport);
	void hyperspace(sf::FloatRect viewport);
	void spawn(SpawnType type, sf::Vector2f pos, EntityID::EntityID ID);

private:
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

	template<typename T>
	void spawnWrapper(sf::Vector2f pos, EntityID::EntityID ID, EntityHolder<T>& entities)
	{
		uint16_t index;

		if (entities.count != 0)
		{
			entities.entities.at(entities.insertionSide ? entities.first : entities.last) = new T(pos, ID);

			--entities.count;

			if (entities.count == 0)
			{
				entities.first = 0;
				entities.last = 0;
				entities.insertionSide = false;
			}
			else
			{
				// search for the next entity on the appropriate side! & flip insertion side
				for (int i = entities.insertionSide
						? entities.last
						: entities.first;
					entities.insertionSide
						? i > entities.first
						: i < entities.last; 
					entities.insertionSide
						? --i
						: ++i)
				{
					if (entities.entities.at(i) == nullptr)
						entities.insertionSide
							? entities.last  = i
							: entities.first = i;
				}
			}
		}
		else
		{
			entities.entities.push_back(nullptr);
			index = entities.entities.size();
		}

		entities.entities.at(index) = new T(pos, ID);
	}

	EntityHolder<Projectile> projectiles;
	EntityHolder<Enemy>      enemies;
	EntityHolder<Astronaut>  astronauts;
	EntityHolder<Entity>     particles; // Always scripted
	Player* player = nullptr;

	bool scripted;
};

