#pragma once
#include <SFML/Graphics.hpp>
#include "../ShaderID.h"


class UserInterface
{
	// ################# WORLD BACKGROUND #################
	class World : public sf::Drawable
	{
		class Component
		{
			enum Type : uint8_t
			{
				UP,
				DOWN,
				FLAT
			};
		public:
			Component(uint8_t type_, uint8_t length_) : type(static_cast<Type>(type_)), length(length_) {}

			uint8_t generate(sf::Image& img, sf::Vector2u& pos, uint16_t maxDx);

		private:
			Type type;
			uint8_t length;
		};
	public:
		static void generate();
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	private:
		static sf::RectangleShape border;
		static sf::Sprite background;
		static sf::Texture* bgTex;
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

	static void clean()
	{
		delete shiftingShader;
		delete flashingShader;
		delete williamsShader;

		delete font;
		delete otherFont;
	}

	// @todo complete shader conversion...
	static const sf::Font& getFont();
	static const sf::Font& getOtherFont();
	static       sf::Shader* getShiftingShader();
	static       sf::Shader* getFlashingShader();
	static       sf::Shader* getWilliamsShader();
	static sf::Shader* getShader(ShaderID::ID ID);
	static const void shaderTick(double deltatime);

	static void drawBackground(sf::RenderTarget&, sf::View&);
	static void drawForeground(sf::RenderTarget&, sf::View&);

private:
	static      World     world         ;
	static      Stars     stars         ;
	static      Minimap   minimap       ;
	static sf ::Font*     font          ;
	static sf ::Text      score         ;
	static sf ::Text      credits       ; // @todo find out if this is necessary...
	static sf::Shader*    shiftingShader;
	static sf::Shader*    flashingShader;
	static sf::Shader*    williamsShader;
	static sf::Glsl::Vec3 brightColors[];
	static sf ::Font*     otherFont     ;
};

