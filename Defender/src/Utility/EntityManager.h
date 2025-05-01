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
		template<typename E>
			requires (std::is_base_of_v<T, E> || std::is_same_v<T, E>)
		uint16_t push(Entity* entity);
		uint16_t getLiveCount() const
		{
			//std::cout << "Live Count: " << entities.size() - count << '\n';
			return entities.size() - count;
		}
		void kill(uint16_t index);
		void reset();
		void zero();

		~EntityHolder();

		uint16_t first = 0, last = 0, count = 0;
	private:

		void getIndex(uint16_t& index);
		uint16_t getNextIndex(uint16_t start, uint16_t extrema, int8_t diff);
		uint16_t findIndex_last_internal();

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
    static bool      tick(double deltatime, float );
    virtual void     draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	static void particleize(bool spawn, sf::Vector2f pos, EntityID::ID ID, sf::Vector2<int8_t> collision, Entity* entity);
	static void      killArea(sf::FloatRect viewport);
	static void      hyperspace(sf::Vector2f size, float left);
	template <typename... Args>
	static void      spawn(EntityID::ID ID, sf::Vector2f pos, Args&&... args);
	static uint8_t astronautCount() { return static_cast<uint8_t>(astronauts.getLiveCount()); }
	static void killAstronauts()
	{
	    astronauts.reset();
	}
	static ScoreType getScore();
	static void waveReset();
	static void deathReset();
	static bool waveComplete();
	static bool playerLiving() { return player; }

	static sf::Vector2f getPlayerPos() 
	{
		if (player)
			return player->getPos();
		return {};
	}

private:
	// @todo If time permits, play with optimization, potentially using a spatial tree.
	template<typename T>
	static bool collisionWrapper(uint16_t entity, EntityHolder<T>& entities);

	static void clearQueue();
	static void spawn_typeWrapper(Entity* entity);

	static void tickLander(double deltatime, uint16_t index);


	static EntityHolder<Projectile> projectiles;
	static EntityHolder<Enemy>      enemies;
	static EntityHolder<Astronaut>  astronauts;
	static EntityHolder<Particle>   particles; // Always scripted
	static Player* player;

	// first is lander     -> astronaut
	// second is astronaut -> lander
	static std::pair<std::unordered_map<uint16_t, uint16_t>, std::unordered_map<uint16_t, uint16_t>> landerTargetTable;

	// @todo Make score update
	static ScoreType score;

	static uint16_t baiterCounter;
	
	static bool scripted;
	static const bool& isInvasion, &spawningComplete;
};

inline void EntityManager::tickLander(double deltatime, uint16_t index)
{
	Lander* entity = dynamic_cast<Lander*>(enemies.entities.at(index));

	if (isInvasion)
	{
		std::cout << "INVASION SPAWN\n";
		spawn(EntityID::MUTANT, entity->getPos());
		spawn(EntityID::MUTANT, entity->getPos());
		enemies.kill(index);
		return;
	}
    
	if (!entity->hasTarget())
	{
		for (uint16_t i = 0; i < astronauts.entities.size(); ++i)
		{
			if (astronauts.entities.at(i) && astronauts.entities.at(i)->targeted())
			{
				entity->setTarget(astronauts.entities.at(i));
				landerTargetTable.first[index] = i;
				landerTargetTable.second[i] = index;
				break;
			}

		}
	}

	enemies.entities.at(index)->tick(deltatime);
}

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
	//std::cout << "KILL! CT: " << count << ", SZ: " << entities.size() << '\n';
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
	uint16_t index = findIndex_last_internal();

	// Place new entity
	entities.at(index) = new E(std::forward<Args>(args)...);

	return index;
}

template<typename T>
template<typename E>
requires (std::is_base_of_v<T, E> || std::is_same_v<T, E>)
uint16_t EntityManager::EntityHolder<T>::push(Entity* entity)
{
	uint16_t index = findIndex_last_internal();

	// Place new entity
	entities.at(index) = dynamic_cast<E*>(entity);

	return index;
}

template<typename T>
uint16_t EntityManager::EntityHolder<T>::findIndex_last_internal()
{
	uint16_t index = 0;

	//std::cout << "SPAWNING AT - (" << pos.x << ", " << pos.y << ")\n";

	//std::cout << "FS: " << first << ", LS: " << last << ", CT: " << count << ", SZ: " << entities.size() << "\n";
	// Generate index
	if (count == 0)
	{
		//std::cout << "ECOUT\n";
		entities.emplace_back(nullptr);
		index = static_cast<uint16_t>(entities.size() - 1);
	}
	else
	{

		//std::cout << "N_ECOUT\n";
		if (count > 1)
			getIndex(index);
		else // count == 1
			index = first;


		--count;
	}
	//std::cout << "FS: " << first << ", LS: " << last << ", CT: " << count << ", SZ: " << entities.size() << " END\n";

	//std::cout << index << '\n';
	// Place new entity
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
void EntityManager::EntityHolder<T>::zero()
{
	for (auto& entity : entities)
	{
		if (entity)
			entity->setPos({ DisplayManager::getView().getCenter().x + COMN::worldSize / 2, entity->getPos().y });
	}
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
		//std::cout << entity_ << ' ' << projectiles.entities.at(entity) << '\n';
		T*& entity_ = entities.entities.at(i);

		if (entity_ != nullptr &&
			projectiles.entities.at(entity)->collide(entity_))
		{
			if constexpr (std::is_same_v<T, Enemy>)
			{
				score += entity_->getXP();

				if (dynamic_cast<Lander*>(entity_))
				{
					if (dynamic_cast<Lander*>(entity_)->hasTarget())
					{
						astronauts.entities.at(landerTargetTable.first[i])->setTargeted(false);
						// Erase the entry pairing this astronaut with the lander
						landerTargetTable.second.erase(landerTargetTable.first[i]);
						// Erase the entry pairing this lander with the astronaut
						landerTargetTable.first.erase(i);
					}
				} else if (dynamic_cast<Baiter*>(entity_))
					--baiterCounter;
				else if (dynamic_cast<Pod*>(entity_))
					for (uint8_t i = 0; i < 5; i++)
						spawn(EntityID::SWARMER, entity_->getPos());
			}
			else if constexpr (std::is_same_v<T, Astronaut>)
			{
				if (dynamic_cast<Astronaut*>(entity_)->targeted())
				{
					dynamic_cast<Lander*>(enemies.entities.at(landerTargetTable.second[i]))->setTarget(nullptr);
					// Erase the entry pairing this astronaut with the lander
					landerTargetTable.first.erase(landerTargetTable.second[i]);
					// Erase the entry pairing this lander with the astronaut
					landerTargetTable.second.erase(i);
				}
			}

			// @todo Find collision point for particalization; Projectile method?
			//particleize(false, entity_->getPos(), entity_->getID());

			projectiles.kill(entity);
			entities.kill(i);

			died = true;
		}

		++i;
	}

	return died;
}


template<typename ... Args>
void EntityManager::spawn(EntityID::ID ID, sf::Vector2f pos, Args&&... args)
{
	Entity* entity;

	switch (ID)
	{

	case EntityID::PLAYER:
		//delete player;
		std::cout << "SPAWNING PLAYER...";
		entity = new Player(pos, args...);
		break;

	case EntityID::ASTRONAUT:
		entity = new Astronaut(pos, args...);
		break;

	case EntityID::BULLET:
		projectiles.push<Bullet>(new Bullet(pos, args...));
		return;

	case EntityID::LASER:
		projectiles.push<Laser>(new Laser(pos, args...));
		return;

	case EntityID::BOMB:
		projectiles.push<Bomb>(new Bomb(pos, args...));
		return;

	case EntityID::LANDER:
		entity = new Lander(pos, args...);
		break;

	case EntityID::MUTANT:
		entity = new Mutant(pos, args...);
		break;

	case EntityID::BAITER:
		++baiterCounter;
		if (player)
    		pos += player->getPos();

		if (pos.x > COMN::worldSize)
			pos.x -= COMN::worldSize;
		else if (pos.x < 0)
			pos.x += COMN::worldSize;

		if (pos.y > COMN::playHeight::max)
			pos.y -= COMN::playHeight::max - COMN::playHeight::min;
		else if (pos.y < COMN::playHeight::min)
			pos.y += COMN::playHeight::max - COMN::playHeight::min;

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
			"Invalid SpawnType : EntityManager::spawn(sw)");
	}

	// @todo complete
	particleize(true, Entity::makeCenteredTL(pos, ID), ID, Particle::defCent(), entity);
}