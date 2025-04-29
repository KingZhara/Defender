#include "../ScannerState.h"

#include <iostream>
#include <fstream>

EntityManager ScannerState::entityMgr;

std::vector<EntityScript*> ScannerState::entityScripts;

ScannerState::ScannerState()
{
	entityMgr = EntityManager(true);

}

ScannerState::~ScannerState()
{
}

bool ScannerState::tick(double deltatime)
{
	return false;
}

void ScannerState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}


void ScannerState::initialize()
{
	loadEntityScripts();
}

void ScannerState::loadEntityScripts()
{
	// Change name later probably
	std::ifstream file("res/test.dscr");
	if (file)
	{
		std::string temp;

		//         |list of tokens in a line|
		std::vector<std::vector<std::string>> tokenLines;

		// Read in all lines and tokenize them.
		// Put the line's tokens into a vector
		while (std::getline(file, temp))
		{
			std::vector<std::string> tokens = tokenizeEntityScript(
				formatEntityScriptLine(temp));
			if (tokens.size() > 0)
				if (tokens[0].size() > 0)
					tokenLines.push_back(tokens);
		}

		EntityScript* lastPtr = nullptr;
		EntityScript* loopPtr = nullptr;
		bool firstNode = true;

		// For each line, try to add a script node
		for (int i = 0; i < tokenLines.size(); i++)
		{
			// Need to get these from makeEntityScriptParams
			bool endScript = false;
			bool setLoop = false;
			EntityScript* script = makeEntityScriptParams(
				tokenLines[i], setLoop, endScript);

			// If script has a valid node to add
			if (script)
			{
				// Only add head node to scripts vector
				if (firstNode)
				{
					firstNode = false;
					entityScripts.push_back(script);
					lastPtr = entityScripts.back();
				}
				else
				{
					// Move lastPtr to next
					lastPtr->next = script;
					lastPtr = lastPtr->next;
				}
			}
			else
			{
				// Change loop pointer later
				// Thing this points to doesn't exist yet
				if (setLoop)
					loopPtr = lastPtr;

				if (endScript)
				{
					// Reset and set final node's next to loopPtr's next

					if (loopPtr)
						lastPtr->next = loopPtr->next;

					loopPtr = nullptr;
					lastPtr = nullptr;
					firstNode = true;
				}
			}

		}
		file.close();
	}
	else
		std::cout << "Failed to open \"res/test.dscr\"! AttractState.cpp loadEntityScripts() line:189\n";
}

std::string ScannerState::formatEntityScriptLine(const std::string& line)
{
	std::string ret;

	for (int i = 0; i < line.length(); i++)
	{
		switch (line[i])
		{
			// Remove whitespace
		case ' ':
		case '\t':
			break;

			// Ignore comments, line done
		case '/':
			return ret;

		default:
			// Only adds char to new string if its not whitespace
			// Lowercase to help with comparing later
			ret.push_back(tolower(line[i]));
		}
	}

	return ret;
}

std::vector<std::string> ScannerState::tokenizeEntityScript(const std::string line)
{
	// There could be a bug with making an extra token if theres only a comma in the line
	// This is only a temporary vector anyways and it is ignored later
	// If its not broke, don't fix it
	std::vector<std::string> tokens;
	tokens.push_back(std::string());
	for (int i = 0; i < line.length(); i++)
	{
		if (line[i] == ',')
		{
			if (tokens[tokens.size() - 1].size() > 0)
				tokens.push_back(std::string());
		}
		else
			tokens[tokens.size() - 1].push_back(line[i]);
	}

	return tokens;
}

EntityScript* ScannerState::makeEntityScriptParams(const std::vector<std::string>& tokens, bool& setLoop, bool& endScript)
{
	if (tokens.size() > 0)
	{
		// Basically a string switch
		// Switch on first token (the instruction)

		// Tokens after index 0 are params
		// Params:
		// int entityID
		// int x
		// int y
		// int scriptID
		if (tokens[0] == "spawn")
		{
			if (tokens.size() > 4)
			{
				return new EntitySpawnScript(
					stoi(tokens[1]),
					sf::Vector2f(
						(float)stoi(tokens[2]),
						(float)stoi(tokens[3])),
					stoi(tokens[4]));
			}
			else
				std::cout << "Not enough parameters in entity script: spawn\n";
		}
		// Params:
		// int x
		// int y
		else if (tokens[0] == "move")
		{
			if (tokens.size() > 2)
			{
				return new EntityMoveScript(
					sf::Vector2f(
						(float)stoi(tokens[1]),
						(float)stoi(tokens[2])));
			}
			else
				std::cout << "Not enough parameters in entity script: move\n";
		}
		// Params: NONE
		else if (tokens[0] == "fire")
		{
			return new EntityFireScript();
		}
		// Params:
		// double time
		else if (tokens[0] == "wait")
		{
			if (tokens.size() > 1)
			{
				return new EntityWaitScript(stod(tokens[1]));
			}
			else
				std::cout << "Not enough parameters in entity script: wait\n";
		}
		// Params: NONE
		// Set loopPtr to next entityScript. Unknown for now, so set to current.
		// After this is done making ths linked list, move the loop pointer down one
		else if (tokens[0] == "loop")
		{
			setLoop = true;
		}
		// Params: NONE
		// Script is done.
		// Set last entityScript next ptr to loopPtr
		else if (tokens[0] == "end")
		{
			endScript = true;
		}
		else
		{
			std::cout << "Invalid operator in entity script: \"" << tokens[0] << "\"";
		}
	}

	return nullptr;
}