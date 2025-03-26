#pragma once
#include <concepts>
#include <vector>
#include "Action.h"
#include "../Entity/Entity.hpp"

using ScoreType = uint64_t;

class EntityManager : public sf::Drawable
{
	template<typename T>
	struct EntityHolder
	{
		std::vector<T*> entities;

		void kill(uint16_t index)
		{
			std::cout << "KILL!\n";
			delete entities.at(index);

			entities.at(index) = nullptr;

			count++;

			if (index < first)
				first = index;
			else if (index > last)
				last = index;
		}

		// For all non-entity specifications
		template<typename E>
		requires (!std::is_same_v<E, Entity> && std::is_base_of_v<T, E>)
		uint16_t spawn(sf::Vector2f pos)
		{
			uint16_t index = 0;

			std::cout << "SPAWNING AT - (" << pos.x << ", " << pos.y << ")\n";

			// Generate index
			if (count == 0)
			{
				entities.emplace_back(nullptr);
				index = static_cast<uint16_t>(entities.size() - 1);
			}
			else
			{
				//std::cout << first << ' ' << last << ' ' << count << '\n';

				if (count > 1)
					getIndex(index);
				else // count == 1
					index = first;


				--count;
			}

			//std::cout << index << '\n';
			// Place new entity
			entities.at(index) = new E(pos);

			return index;
		}

		// Specifically for the entity specification, used solely for particles
		template<typename E>
		requires (std::is_same_v<E, Entity>)
		uint16_t spawn(sf::Vector2f pos, sf::IntRect bounds)
		{
			uint16_t index = 0;

			// Generate index
			if (count == 0)
			{
				entities.emplace_back(nullptr);
				index = static_cast<uint16_t>(entities.size() - 1);
			}
			else
			{
				//std::cout << first << ' ' << last << ' ' << count << '\n';

				if (count > 1)
					getIndex(index);
				else // count == 1
					index = first;


				--count;
			}

			// Place new entity
			entities.at(index) = new E(pos, bounds);

			return index;
		}

		~EntityHolder()
		{
			for (auto& entity : entities)
				delete entity;
		}

		void reset()
		{
			for (auto& entity : entities)
				delete entity;

			entities.clear();
			entities.shrink_to_fit();
		}

	private:

		void getIndex(uint16_t& index)
		{
			if (count < 2)
				throw std::runtime_error("Invalid count during call : uint16_t EntityManager::EntityHolder::getIndex()");

			// Generate the index
			if (count == 2)
			{
				// Assign it to one, make both the same
				if (insertionSide)
				{
					index = first;
					first = last;
				}
				else
				{
					index = last;
					last = first;
				}
			}
			else
			{
				// Assign it to one, find the next index for that side
				index = insertionSide ? first : last;
				insertionSide ? first : last = insertionSide ? getNextIndex(first, last, 1)
															 : getNextIndex(last, first, -1);
			}
		}

		uint16_t getNextIndex(uint16_t start, uint16_t extrema, int8_t diff)
		{
			// Search for the first nullptr in selected direction(diff)
			do
				start += diff;
			while (start != extrema - diff && entities.at(start) != nullptr);

			// Return the first found
			return start;
		}

		// Dead entity search information
		uint16_t first = 0, last = 0, count = 0;
		bool insertionSide = false;
	};

public:
	enum class SpawnType
	{
		PLAYER,
		PROJECTILE,
		ENEMY,
		ASTRONAUT,
	};

	EntityManager(bool scripted_ = false);

	~EntityManager() override = default;
    static void adjViewport(sf::View * view);
    /*
	void test()
	{
		enemies.kill(2);
		enemies.kill(5);
		enemies.kill(6);
		enemies.kill(1);
	}*/

    static bool tick(Action& actions, double deltatime, float );
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	static void particleize(bool spawn, sf::Vector2f pos, EntityID::ID ID);
	static void killArea(sf::FloatRect viewport);
	static void hyperspace(sf::Vector2f size, float left);
	static void spawn(SpawnType type, sf::Vector2f pos, EntityID::ID ID);
	static ScoreType getScore()
	{
		return score;
	}

private:
	// @todo If time permits, play with optimization, potentially using a spatial tree.
	template<typename T>
	static bool collisionWrapper(uint16_t entity, EntityHolder<T>& entities)
    {
		uint16_t i = 0;
		bool died = false;

		while (i < entities.entities.size() && died != true)
		{
			//std::cout << entities.entities.at(i) << ' ' << projectiles.entities.at(entity) << '\n';

			if (entities.entities.at(i) != nullptr &&
				projectiles.entities.at(entity)->collide(entities.entities.at(i)))
			{
				//particleize(false, entities.entities.at(i)->getPos(), entities.entities.at(i)->getID());

				projectiles.kill(entity);
				entities.kill(i);

				died = true;
			}

			++i;
		}

		return died;
	}

	static void clearQueue();


	static EntityHolder<Projectile> projectiles;
	static EntityHolder<Enemy>      enemies;
	static EntityHolder<Astronaut>  astronauts;
	static EntityHolder<Entity>     particles; // Always scripted
	static Player* player;

	// @todo Make score update
	static ScoreType score;
	static bool scripted;
};