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
		uint16_t spawn(uint16_t index, T* entity);
		template<typename E>
			requires (std::is_base_of_v<T, E> || std::is_same_v<T, E>)
		uint16_t push(Entity* entity);
		uint16_t getLiveCount() const
		{
			return static_cast<uint16_t>(entities.size() - freeIndices.size());
		}

		uint16_t getDeadCount() const
		{
			return static_cast<uint16_t>(freeIndices.size());
		}
		void kill(uint16_t index);
		void reset();
		void zero();

		~EntityHolder();
	private:
		std::deque<uint16_t> freeIndices; // Tracks free indices
	};

public:
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
	static EntityHolder<Entity>     enemies;
	static EntityHolder<Entity>     astronauts;
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
			Astronaut* astronaut = dynamic_cast<Astronaut*>(astronauts.entities.at(i));
			if (astronaut && astronaut->targeted())
			{
				entity->setTarget(astronaut);
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
	delete entities[index];
	entities[index] = nullptr;
	freeIndices.push_back(index);
}

template<typename T>
template<typename E, typename ... Args>
	requires (std::is_base_of_v<T, E> || std::is_same_v<T, E>)
uint16_t EntityManager::EntityHolder<T>::spawn(Args &&...args)
{
	uint16_t index;

	if (!freeIndices.empty()) {
		index = freeIndices.front();
		freeIndices.pop_front();
	}
	else {
		index = static_cast<uint16_t>(entities.size());
		entities.emplace_back(nullptr);
	}

	entities[index] = new E(std::forward<Args>(args)...);
	return index;
}

template<typename T>
uint16_t EntityManager::EntityHolder<T>::spawn(uint16_t index, T* entity)
{
	delete entities.at(index);
	// Place new entity
	entities.at(index) = entity;

	return index;
}

template<typename T>
template<typename E>
requires (std::is_base_of_v<T, E> || std::is_same_v<T, E>)
uint16_t EntityManager::EntityHolder<T>::push(Entity* entity)
{
	uint16_t index;

	if (!freeIndices.empty()) {
		index = freeIndices.front();
		freeIndices.pop_front();
	}
	else {
		index = static_cast<uint16_t>(entities.size());
		entities.emplace_back(nullptr);
	}

	entities[index] = dynamic_cast<E*>(entity);
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
	for (T*& e : entities) {
		delete e;
		e = nullptr;
	}
	freeIndices.clear();
	for (uint16_t i = 0; i < entities.size(); ++i)
		freeIndices.push_back(i);
	for (T*& e : entities) {
		delete e;
		e = nullptr;
	}
	freeIndices.clear();
	for (uint16_t i = 0; i < entities.size(); ++i)
		freeIndices.push_back(i);
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
bool EntityManager::collisionWrapper(uint16_t entity, EntityHolder<T> &entities)
{
	uint16_t i = 0;
	bool died = false;

	while (i < entities.entities.size() && died != true)
	{
		//std::cout << entity_ << ' ' << projectiles.entities.at(entity) << '\n';
		T* entity_ = entities.entities.at(i);

		if (entity_ != nullptr && // If the entity is not dead
		 !((projectiles.entities.at(entity)->getID() == EntityID::LASER && entity_->getID() == EntityID::PLAYER) || // If its not a laser hitting a player
		   (projectiles.entities.at(entity)->getID() != EntityID::LASER && entity_->getID() != EntityID::PLAYER)) && // or a non laser hitting a non player
			projectiles.entities.at(entity)->collide(entity_)) // and the collision is happening
		{
			if (dynamic_cast<Enemy*>(entity_))
			{
				score += entity_->getXP();

				if (dynamic_cast<Lander*>(entity_))
				{
					Astronaut* astronaut = dynamic_cast<Astronaut*>(astronauts.entities.at(landerTargetTable.first[i]));
					if (astronaut && dynamic_cast<Lander*>(entity_)->hasTarget())
					{
						dynamic_cast<Astronaut*>(astronauts.entities.at(landerTargetTable.first[i]))->setTargeted(false);
						// Erase the entry pairing this astronaut with the lander
						landerTargetTable.second.erase(landerTargetTable.first[i]);
						// Erase the entry pairing this lander with the astronaut
						landerTargetTable.first.erase(i);
					}
				}
				else if (dynamic_cast<Pod*>(entity_))
				{
					for (uint8_t j = 0; j < 5; j++)
					{
						spawn(EntityID::SWARMER, entity_->getPos());
					    entity_ = entities.entities.at(i);
					}
				}
				else if (dynamic_cast<Baiter*>(entity_))
					--baiterCounter;
			}
			else if (dynamic_cast<Astronaut*>(entity_))
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
			particleize(
				false,
				entity_->getPos(),
				entity_->getID(),
				{
					(int8_t)(projectiles.entities.at(entity)->getPos().x - entity_->getPos().x),
					(int8_t)(projectiles.entities.at(entity)->getPos().y - entity_->getPos().y)
				},
				nullptr);

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
	std::cout << "SPAWNING..\n.";
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