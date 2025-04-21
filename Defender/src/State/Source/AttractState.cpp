#include "../AttractState.h"
#include "../../Utility/UserInterface/UserInterface.h"
#include "../../Utility/common.h"
#include "../../Utility/Timer.h"

#include <iostream>
#include <fstream>

#include "../../Utility/DisplayManager.h"


EntityManager AttractState::entityMgr;

std::vector<EntityScript*> AttractState::entityScripts;

sf::Image AttractState::willSteps, AttractState::willImg;
sf::Texture* AttractState::willTex;
sf::RectangleShape AttractState::williams;

sf::Text AttractState::electronicsInc, AttractState::presents, 
AttractState::copyright, AttractState::credits;

sf::Texture* AttractState::defenderTex;
sf::RectangleShape AttractState::defender;

sf::RenderTexture* AttractState::flashing, * AttractState::shifting, * AttractState::willFlashing;

sf::Sprite AttractState::flshDra, AttractState::shftDra, AttractState::willFlshDra;


AttractState::AttractState()
{
	entityMgr = EntityManager(true);

	electronicsInc.setFillColor(sf::Color(0));
	presents.setFillColor(sf::Color(0));
	defender.setFillColor(sf::Color(0));
	copyright.setFillColor(sf::Color(0));
	credits.setFillColor(sf::Color(0));

	willImg.create(willSteps.getSize().x + 1, willSteps.getSize().y, sf::Color(0));

	shifting->clear(sf::Color(0));
	flashing->clear(sf::Color(0));
	willFlashing->clear(sf::Color(0));
}

void AttractState::initialize()
{
	willTex = new sf::Texture;
	defenderTex = new sf::Texture;
	flashing = new sf::RenderTexture;
	shifting = new sf::RenderTexture;
	willFlashing = new sf::RenderTexture;

	willSteps.loadFromFile("res/williams.png");

	williams.setSize(sf::Vector2f((float)willSteps.getSize().x, (float)willSteps.getSize().y));
	williams.setPosition(COMN::resolution.x / 2 - williams.getGlobalBounds().getSize().x / 2, 10);

	willImg.create(willSteps.getSize().x + 1, willSteps.getSize().y, sf::Color(0));
	willTex->create(willSteps.getSize().x + 1, willSteps.getSize().y);

	electronicsInc.setFont(UserInterface::getFont());
	electronicsInc.setString("ELECTRONICS INC.");
	electronicsInc.setCharacterSize(16);
	electronicsInc.setPosition(COMN::resolution.x / 2 - (18 * 16 / 4), 50);

	presents.setFont(UserInterface::getFont());
	presents.setCharacterSize(16);
	presents.setString("PRESENTS");
	presents.setPosition(COMN::resolution.x / 2 - (18 * 8 / 4), 60);


	defenderTex->loadFromFile("res/defender.png");
	defender.setSize(sf::Vector2f(defenderTex->getSize()) / 2.f);
	defender.setTexture(defenderTex);
	defender.setPosition(COMN::resolution.x / 2 - defender.getGlobalBounds().getSize().x / 2, 90);


	copyright.setFont(UserInterface::getOtherFont());
	copyright.setCharacterSize(16);
	copyright.setString("COPYRIGHT @ 1980");
	copyright.setPosition(COMN::resolution.x / 2 - 40, 150);

	credits.setFont(UserInterface::getFont());
	credits.setCharacterSize(16);
	credits.setString("CREDITS: 00");
	credits.setPosition(COMN::resolution.x / 4, 160);

	flashing->create((unsigned)COMN::resolution.x, (unsigned)COMN::resolution.y);
	shifting->create((unsigned)COMN::resolution.x, (unsigned)COMN::resolution.y);
	willFlashing->create((unsigned)COMN::resolution.x, (unsigned)COMN::resolution.y);

	flashing->setView(DisplayManager::getView());
	shifting->setView(DisplayManager::getView());
	willFlashing->setView(DisplayManager::getView());

	flshDra.setTexture(flashing->getTexture());
	shftDra.setTexture(shifting->getTexture());
	willFlshDra.setTexture(willFlashing->getTexture());

	flshDra.setScale(1.f, -1.f);
	shftDra.setScale(1.f, -1.f);
	willFlshDra.setScale(1.f, -1.f);

	flshDra.move(0, COMN::resolution.y);
	shftDra.move(0, COMN::resolution.y);
	willFlshDra.move(0, COMN::resolution.y);

	loadEntityScripts();
}


AttractState::~AttractState()
{
}

bool AttractState::tick(double deltatime)
{
	static Timer<double> textTimer{ 0.03 };
	static Timer<int> stageTimer{80, false};

	while (textTimer.tick(deltatime))
	{
		if (!stageTimer.isComplete())
		{
			//std::cout << stage << '\n';
			stageTimer.tick(1);
		}
		else
		{
			switch (stage)
			{
			case 0: // Williams

				willPos++;
				for (unsigned x = 0; x < willSteps.getSize().x; x++)
					for (unsigned y = 0; y < willSteps.getSize().y; y++)
						if (willSteps.getPixel(x, y).toInteger() < willPos && 
							willSteps.getPixel(x, y).toInteger())
							willImg.setPixel(x, y, COMN::ShaderTarget);

				willTex->update(willImg);
				williams.setTexture(willTex);

				if (willPos > 110)
				{
					willPos = 0;
					stage++;
					stageTimer.tick(1); // Will reset
				}

				break;

			case 1: // Electronics Inc presents
				electronicsInc.setFillColor(sf::Color(COMN::ShaderTarget));
				presents.setFillColor(COMN::ShaderTarget);
				stage++;
				stageTimer.tick(1);
				break;

			case 2: // Defender
				defender.setFillColor(sf::Color::White);
				stage++;
				stageTimer.tick(1);
				break;

			case 3: // Copyright credits
				copyright.setFillColor(COMN::ShaderTarget);
				credits.setFillColor(COMN::ShaderTarget);
				stage++;
				stageTimer.addTime((int)(1. / textTimer.getBase() * 5.));
				stageTimer.tick(1);
				break;

			case 4:
				return true;
			}
		}
		deltatime = 0;
	}
	return false;
}

void AttractState::loadEntityScripts()
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

std::string AttractState::formatEntityScriptLine(const std::string& line)
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

std::vector<std::string> AttractState::tokenizeEntityScript(const std::string line)
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

EntityScript* AttractState::makeEntityScriptParams(const std::vector<std::string>& tokens, bool& setLoop, bool& endScript)
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
						stoi(tokens[2]),
						stoi(tokens[3])),
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
						stoi(tokens[1]), 
						stoi(tokens[2])));
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