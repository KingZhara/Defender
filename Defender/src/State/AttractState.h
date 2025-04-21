#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_set>

#include "../Utility/UserInterface/UserInterface.h"
#include "../Utility/common.h"
#include "../Utility/EntityManager.h"

class AttractState : public sf::Drawable
{
public:

	AttractState();

	static void initialize();

	static void clean()
	{
		delete willTex;
		delete defenderTex;
		delete flashing;
		delete shifting;
		delete willFlashing;

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

	~AttractState();

	bool tick(double deltatime);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		switch (stage)
		{
		case 4:
			UserInterface::getShiftingShader()->setUniform("texture", sf::Shader::CurrentTexture);
			UserInterface::getFlashingShader()->setUniform("texture", sf::Shader::CurrentTexture);
			UserInterface::getWilliamsShader()->setUniform("texture", sf::Shader::CurrentTexture);
			[[fallthrough]];

		case 3: // Copyright
			shifting->draw(copyright, states);
			shifting->draw(credits, states);
			[[fallthrough]];

		case 2: // Defender
			UserInterface::getFlashingShader()->setUniform("targetColor", sf::Glsl::Vec3(1.f, 1.f, 0.f));
			flashing->draw(defender, states);
			[[fallthrough]];

		case 1: // Electronics Inc
			shifting->draw(electronicsInc, states);
			shifting->draw(presents, states);
			[[fallthrough]];

		case 0: // Williams
			// Yes, this needs to be here otherwise it will be fully drawn after the first time thru
			willFlashing->clear(sf::Color(0)); 
			willFlashing->draw(williams, states); // Will require alternate shader
		}

		states.shader = UserInterface::getFlashingShader();
		target.draw(flshDra, states);
		UserInterface::getFlashingShader()->setUniform("targetColor", COMN::ShaderTarget);


		states.shader = UserInterface::getShiftingShader();
		target.draw(shftDra, states);

		states.shader = UserInterface::getWilliamsShader();
		target.draw(willFlshDra, states);
	}

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

	static sf::Image willSteps;
	static sf::RectangleShape williams;

	static sf::Text electronicsInc, presents, copyright, credits;

	static sf::Texture *defenderTex;
	static sf::RectangleShape defender;

	static sf::RenderTexture *flashing, *shifting, *willFlashing;

	static sf::Sprite flshDra, shftDra, willFlshDra;

	// needs to be recreated each time
	// The width of these is actually 1 less because the shader cut off the final pixels
	// For width use willSteps
	static sf::Image willImg;
	static sf::Texture* willTex;
	
	static EntityManager entityMgr;

	unsigned willPos = 0;

	int stage = 0;
	
};

