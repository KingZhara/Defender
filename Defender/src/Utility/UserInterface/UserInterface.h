#pragma once
#include <SFML/Graphics.hpp>

#include "Minimap.h"
#include "Stars.h"


class UserInterface : public sf::Drawable
{
	class World
	{
		class Component
		{
			enum class type
			{
				UP,
				DOWN,
				FLAT
			};
		public:
		private:
		};
	public:
	private:
	};
public:
	UserInterface()
	{
		if (!initialized)
		{
			font.loadFromFile("res/defender.ttf");
			flashingShader.loadFromFile("res/shaders/flashing", sf::Shader::Type::Fragment);
			shiftingShader.loadFromFile("res/shaders/shifting", sf::Shader::Type::Fragment);

			initialized = true;
		}
		
	}

	static const sf::Font& getFont()
	{
		return font;
	}

	static const sf::Shader& getFlashingShader()
	{
		return flashingShader;
	}

	static const sf::Shader& getShiftingShader()
	{
		return shiftingShader;
	}

	void drawBackground(sf::RenderTarget&, sf::View&);
	void drawForeground(sf::RenderTarget&, sf::View&);

private:
	static     bool    initialized   ;
	static     World   world         ;
	static     Stars   stars         ;
	static     Minimap minimap       ;
	static sf::Font    font          ;
	static sf::Text    score         ;
	static sf::Text    credits       ; // @todo find out if this is necessary...
	static sf::Shader  flashingShader;
	static sf::Shader  shiftingShader;

};

