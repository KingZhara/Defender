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
		uint16_t first = 0, last = 0, count = 0;
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

		void kill(uint16_t index)
		{
			delete entities.at(index);

			entities.at(index) = nullptr;

			updateData(true, index);
		}

		void spawn(sf::Vector2f pos, EntityID::EntityID ID)
		{
			uint16_t index;

			if (count != 0)
			{
				entities.at(insertionSide ? first : last) = new T(pos, ID);

				--count;

				// search for the next entity on the appropriate side! & flip insertion side
				// @todo convert to the appropriate do-while loop
				for (int i = insertionSide
						? last
						: first;
					insertionSide
						? i > first
						: i < last;
					insertionSide
						? --i
						: ++i)
				{
					if (entities.at(i) == nullptr)
					{
						insertionSide
							? last = i
							: first = i;
						index = i;
					}
				}
			}
			else
			{
				index = entities.size();
				entities.push_back(nullptr);
			}

			entities.at(index) = new T(pos, ID);
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

	
	bool tick(Action& actions, double deltatime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void particleize(bool spawn, sf::Vector2f pos, EntityID::EntityID ID);
	void killArea(sf::FloatRect viewport);
	void hyperspace(sf::FloatRect viewport);
	void spawn(SpawnType type, sf::Vector2f pos, EntityID::EntityID ID);

private:
	// @todo If time permits, play with optimization, potentially using a spacial tree.
	template<typename T>
	void collisionWrapper(uint16_t entity, EntityHolder<T>& entities) {
		for (uint16_t i = 0; i < entities.entities.size(); i++)
		{
			if (entities.entities.at(i) != nullptr &&
				entities.entities.at(i)->collide(projectiles.entities.at(entity)))
			{
				particleize(false, entities.entities.at(i)->getPos(), entities.entities.at(i)->getID());

				projectiles.kill(entity);
				entities.kill(i);
			}
		}
	}

	EntityHolder<Projectile> projectiles;
	EntityHolder<Enemy>      enemies;
	EntityHolder<Astronaut>  astronauts;
	EntityHolder<Entity>     particles; // Always scripted
	Player* player = nullptr;

	bool scripted;
};

