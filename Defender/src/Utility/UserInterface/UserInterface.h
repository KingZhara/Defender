#pragma once
#include <SFML/Graphics.hpp>


class UserInterface
{
	// ################# WORLD BACKGROUND #################
	class World : public sf::Drawable
	{
		class Component
		{
			enum class Type
			{
				UP,
				DOWN,
				FLAT
			};
		public:
		private:
			Component* next;
			Type type;
			uint8_t length;
		};
	public:
		static void generate();
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		static sf::RectangleShape border;
		static sf::Image background;
	};
	// ################# WORLD BACKGROUND END #############

	// ################# MINIMAP ##########################
	class Minimap : public sf::Drawable
	{
	public:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
	};
	// ################# MINIMAP END ######################

	// ################# STARS BACKGROUND #################
	class Stars : public sf::Drawable
	{
	public:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
	};
	// ################# STARS BACKGROUND END #############

public:
	UserInterface() = delete;

	static void initialize();

	// @todo complete shader conversion...
	static const sf::Font& getFont();
	static       sf::Shader* getShiftingShader();
	static const void shaderTick(double deltatime);

	static void drawBackground(sf::RenderTarget&, sf::View&);
	static void drawForeground(sf::RenderTarget&, sf::View&);

private:
	static      World   world         ;
	static      Stars   stars         ;
	static      Minimap minimap       ;
	static sf ::Font    font          ;
	static sf ::Text    score         ;
	static sf ::Text    credits       ; // @todo find out if this is necessary...
	static sf ::Shader* shiftingShader;

};

