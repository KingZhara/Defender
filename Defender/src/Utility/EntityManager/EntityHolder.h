#pragma once
#include <cstdint>
#include <deque>
#include <vector>
#include "../EntityManager.h"

class Entity;

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


template<typename T>
void EntityHolder<T>::kill(uint16_t index)
{
	delete entities[index];
	entities[index] = nullptr;
	freeIndices.push_back(index);
}

template<typename T>
template<typename E, typename ... Args>
	requires (std::is_base_of_v<T, E> || std::is_same_v<T, E>)
uint16_t EntityHolder<T>::spawn(Args &&...args)
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
uint16_t EntityHolder<T>::spawn(uint16_t index, T* entity)
{
	delete entities.at(index);
	// Place new entity
	entities.at(index) = entity;

	return index;
}

template<typename T>
template<typename E>
	requires (std::is_base_of_v<T, E> || std::is_same_v<T, E>)
uint16_t EntityHolder<T>::push(Entity* entity)
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
EntityHolder<T>::~EntityHolder()
{
	for (auto& entity : entities)
		delete entity;
}

template<typename T>
void EntityHolder<T>::reset()
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
void EntityHolder<T>::zero()
{
	for (auto& entity : entities)
	{
		if (entity)
			entity->setPos({ DisplayManager::getView().getCenter().x + COMN::worldSize / 2, entity->getPos().y });
	}
}