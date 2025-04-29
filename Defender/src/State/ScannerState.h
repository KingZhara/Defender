#pragma once

#include <SFML/Graphics.hpp>

#include <unordered_set>

#include "../Utility/UserInterface/UserInterface.h"
#include "../Utility/DisplayManager.h"

#include "../Utility/common.h"
#include "../Utility/EntityManager.h"

#include "../Utility/EntityScript/EntityMoveScript.h"
#include "../Utility/EntityScript/EntityWaitScript.h"
#include "../Utility/EntityScript/EntitySpawnScript.h"
#include "../Utility/EntityScript/EntityFireScript.h"
#include "../Utility/EntityScript/EntityScript.h"

class ScannerState : public sf::Drawable
{
public:

	ScannerState();

	~ScannerState();

	bool tick(double deltatime);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	static void clean()
	{
		// Following next pointers doesn't work with looping scripts
		// The ending node's next pointer is the loop start
		// Have to make a list of pointers its already visited
		// If it visits a pointer its already been to, then its finished

		std::unordered_set<void*> visited;
		EntityScript* script = nullptr;
		for (int i = 0; i < entityScripts.size(); i++)
		{
			visited.clear();
			script = entityScripts[i];
			while (!visited.contains((void*)script) && script)
			{
				visited.insert((void*)script);
				EntityScript* temp = script->next;
				delete script;
				script = temp;
			}
		}
	}

	static void initialize();

private:

	// This method calls all the other entity script loading methods
	// Only call this one, not the other entity script methods
	static void loadEntityScripts();

	// First step of loading an entity script.
	// It removes single line comments and whitespace from the source file.
	static std::string formatEntityScriptLine(const std::string& line);

	// Second step of loading an entity script.
	// This makes a list of substrings from a line after it went thru the format method (seperated by commas).
	static std::vector<std::string> tokenizeEntityScript(const std::string line);

	// Called for each line in a script file
	// It is its own function, but I needed to know if a node was a loop or end instruction without testing that again
	// This fills in entityScript parameters
	static EntityScript* makeEntityScriptParams(const std::vector<std::string>& tokens, bool& setLoop, bool& endScript);

	static std::vector<EntityScript*> entityScripts;

	static EntityManager entityMgr;


};