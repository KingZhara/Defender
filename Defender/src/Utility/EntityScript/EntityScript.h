#pragma once
#include <cstdint>
#include <stdexcept>
#include <SFML/System/Vector2.hpp>

// @todo figure out why size seems off...
struct EntityScript
{
	enum class ScriptType : uint8_t
	{
		DONE, // Should probs be changed to a next == nullptr check IR no EntityScript::DONE val.
		FIRE,
		WAIT,
		MOVE,
		SPAWN
	};

	// The next component of the script
	EntityScript* next = nullptr;
	EntityScript* loop = nullptr;
	// The type of script
	ScriptType type;

	union
	{
		struct
		{
			int x;
			int y;
			int entity;
			int script;
		} spawn;
		struct
		{
			int x;
			int y;
		} move;
		double wait;
	} param;

	EntityScript(ScriptType type_, EntityScript* next_ = nullptr)
		: type(type_), next(next_) {}
	virtual ~EntityScript() = default; // next is handled externally


	virtual bool tick(sf::Vector2f pos = {}, double deltatime = 0.)
	{
		switch (type)
		{
		case ScriptType::DONE:
		case ScriptType::FIRE:
			return true;
		default:
			throw std::runtime_error("Invalid type in base script type!");
		}
	}
};

