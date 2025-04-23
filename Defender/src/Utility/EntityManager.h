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

		template<typename E, typename... Args>
			requires (std::is_base_of_v<T, E> || std::is_same_v<T, E>)
		uint16_t spawn(Args&&... args);
		void kill(uint16_t index);
		void reset();

		~EntityHolder();

		uint16_t first = 0, last = 0, count = 0;
	private:

		void getIndex(uint16_t& index);
		uint16_t getNextIndex(uint16_t start, uint16_t extrema, int8_t diff);

		// Dead entity search information
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
    /*
	void test()
	{
		enemies.kill(2);
		enemies.kill(5);
		enemies.kill(6);
		enemies.kill(1);
	}*/

	static void adjViewport(sf::View* view, double deltatime);
    static bool      tick(Action& actions, double deltatime, float );
    virtual void     draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	static void particleize(bool spawn, sf::Vector2f pos, EntityID::ID ID, sf::Vector2<int8_t> collision, Entity* entity);
	static void      killArea(sf::FloatRect viewport);
	static void      hyperspace(sf::Vector2f size, float left);
	template <typename... Args>
	static void      spawn(SpawnType type, EntityID::ID ID, sf::Vector2f pos, Args&&... args);
	static ScoreType getScore();

private:
	// @todo If time permits, play with optimization, potentially using a spatial tree.
	template<typename T>
	static bool collisionWrapper(uint16_t entity, EntityHolder<T>& entities);

	static void clearQueue();


	static EntityHolder<Projectile> projectiles;
	static EntityHolder<Enemy>      enemies;
	static EntityHolder<Astronaut>  astronauts;
	static EntityHolder<Particle>   particles; // Always scripted
	static Player* player;

	// @todo Make score update
	static ScoreType score;
	static bool scripted;
};

// ################################################
// ################################################
// ################################################
// ################# TEMPLATES ####################
// ################################################
// ################################################
// ################################################

template<typename T>
void EntityManager::EntityHolder<T>::kill(uint16_t index)
{
	std::cout << "KILL! CT: " << count << ", SZ: " << entities.size() << '\n';
	delete entities.at(index);

	entities.at(index) = nullptr;

	count++;

	if (index < first)
		first = index;
	else if (index > last)
		last = index;
}

template<typename T>
template<typename E, typename ... Args>
requires (std::is_base_of_v<T, E> || std::is_same_v<T, E>)
uint16_t EntityManager::EntityHolder<T>::spawn(Args &&...args)
{
	uint16_t index = 0;

	//std::cout << "SPAWNING AT - (" << pos.x << ", " << pos.y << ")\n";

	std::cout << "FS: " << first << ", LS: " << last << ", CT: " << count << ", SZ: " << entities.size() << "\n";
	// Generate index
	if (count == 0)
	{
		std::cout << "ECOUT\n";
		entities.emplace_back(nullptr);
		index = static_cast<uint16_t>(entities.size() - 1);
	}
	else
	{

		std::cout << "N_ECOUT\n";
		if (count > 1)
			getIndex(index);
		else // count == 1
			index = first;


		--count;
	}
	std::cout << "FS: " << first << ", LS: " << last << ", CT: " << count << ", SZ: " << entities.size() << " END\n";

	//std::cout << index << '\n';
	// Place new entity
	entities.at(index) = new E(std::forward<Args>(args)...);

	return index;
}

template<typename T>
EntityManager::EntityHolder<T>::~EntityHolder()
{
	for (auto& entity : entities)
		delete entity;
}

template<typename T>
void EntityManager::EntityHolder<T>::reset()
{
	for (auto& entity : entities)
		delete entity;

	entities.clear();
	entities.shrink_to_fit();

	first = 0;
	last = 0;
	count = 0;
}

template<typename T>
void EntityManager::EntityHolder<T>::getIndex(uint16_t &index)
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

template<typename T>
uint16_t EntityManager::EntityHolder<T>::getNextIndex(uint16_t start,
        uint16_t extrema,
        int8_t diff)
{
	// Search for the first nullptr in selected direction(diff)
	do
		start += diff;
	while (start != extrema - diff && entities.at(start) != nullptr);

	// Return the first found
	return start;
}

template<typename T>
bool EntityManager::collisionWrapper(uint16_t entity, EntityHolder<T> &entities)
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


template<typename ... Args>
void EntityManager::spawn(SpawnType type, EntityID::ID ID, sf::Vector2f pos, Args&&... args)
{
	Entity* entity = nullptr;

	switch (type)
	{
	case SpawnType::PROJECTILE:
		switch (ID)
		{
		case EntityID::BULLET:
			entity = new Bullet(pos, args...);
			break;

		case EntityID::LASER:
			entity = new Laser(pos, args...);
			break;

		case EntityID::BOMB:
			entity = new Bomb(pos, args...);
			break;

		default:
			throw std::runtime_error(
				"Invalid Type : EntityManager::spawn;PROJ(sw)");
		}
		break;
	case SpawnType::ENEMY:
		switch (ID)
		{
		case EntityID::LANDER:
			entity = new Lander(pos, args...);
			break;

		case EntityID::MUTANT:
			entity = new Mutant(pos, args...);
			break;

		case EntityID::BAITER:
			entity = new Baiter(pos, args...);
			break;

		case EntityID::BOMBER:
			entity = new Bomber(pos, args...);
			break;

		case EntityID::POD:
			entity = new Pod(pos, args...);
			break;

		case EntityID::SWARMER:
			entity = new Swarmer(pos, args...);
			break;

		default:
			throw std::runtime_error(
				"Invalid Type : EntityManager::spawn;ENEM(sw)");
		}
		break;
	case SpawnType::ASTRONAUT:
		entity = new Astronaut(pos, args...);
		break;
	case SpawnType::PLAYER:
		//delete player;
		entity = new Player(pos, args...);
		break;

	default:
		throw std::runtime_error(
			"Invalid SpawnType : EntityManager::spawn(sw)");
	}

	// @todo complete
	particleize(true, Entity::makeCenteredTL(pos, ID), ID, Particle::defCent(), entity);
}