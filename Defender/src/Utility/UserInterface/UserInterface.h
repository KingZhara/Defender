#pragma once
#include <SFML/Graphics.hpp>

#include "HSV.h"

#include "../ShaderID.h"
#include "../common.h"
#include "../Timer.h"


class UserInterface
{
	// ################# WORLD BACKGROUND #################
	class World : public sf::Drawable
	{
		struct Component
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


	// ################# STARS BACKGROUND #################
	class Stars : public sf::Drawable
	{
	public:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
	};
	// ################# STARS BACKGROUND END #############

public:
	struct UIBounds
	{
		static constexpr uint8_t MINIMAP_WIDTH = 120;
		// 1/7 of the screen height, minus the 2 pixels below, minus the 1 pixel above
		static constexpr uint8_t MINIMAP_HEIGHT = COMN::resolution.y / 7 - 2 - 1;
		static const sf::Vector2f minimapPosConversion;

		static constexpr uint8_t DIVIDER_WIDTH = 2;
		static constexpr uint8_t LIFE_SPACING = 2;
		static constexpr uint8_t BOMB_SPACING = 1;
		static constexpr uint8_t SIDE_SPACING = (COMN::resolution.x - 2 * DIVIDER_WIDTH - MINIMAP_WIDTH) / 2;
		static constexpr uint8_t MINIMAP_X = SIDE_SPACING + DIVIDER_WIDTH;
		static constexpr uint8_t MINIMAP_Y = MINIMAP_HEIGHT + 1;
	};

	struct EntityManagerData
	{
		const static uint32_t& wave;

		uint8_t extraLives = 2, smartBombs = 3;
		int16_t minimapOffset = 0;
		uint64_t score = 0;
	};

	UserInterface() = delete;

	static void initialize();

	static void clean()
	{
		delete shiftingShader;
		delete flashingShader;
		delete williamsShader;
		delete deathShader;

		delete font;
		delete otherFont;
	}

	// @todo complete shader conversion...
	static const sf::Font& getFont();
	static const sf::Font& getOtherFont();
	//static       sf::Shader* getShiftingShader();
	//static       sf::Shader* getFlashingShader();
	//static       sf::Shader* getWilliamsShader();
	//static sf::Shader* getDeathShader();
	static sf::Shader* getShader(ShaderID::ID ID);
	static const void shaderTick(double deltatime);
	static sf::Color getColor()
	{
		return shiftReplacement;
	}
	static sf::Color resetDeathColor()
	{
		return deathReplacement.color;
	}
	static sf::RenderTarget* getMiniTarget()
	{
		return miniTarget;
	}
	static void startDeathAnimation()
	{
		deathReplacement.active = true;
	}
	static bool isDeathAnimCompleted()
	{
		return !deathReplacement.active;
	}

	static void drawBackground(sf::RenderTarget&, sf::View&);
	static void drawForeground(sf::RenderTarget&, sf::View&, EntityManagerData& data);

private:
	struct DeathReplacement
	{
		bool active = false, passedWhite = false, passedHue = false;
		HSV color;
		float     shift, fadeFactor;
		Timer<double> whiteTime{ 2 };
	};

	static void repeatElement(sf::IntRect texBounds, sf::Vector2f pos, sf::Vector2f diff, uint8_t count, sf::RenderTarget& target);


	static      World         world           ;
	static      Stars         stars           ;
	static sf ::Font*         font            ;
	static sf ::Text          score           ;
	static sf ::Text          credits         ; // @todo find out if this is necessary...
	static sf::Shader*        shiftingShader  ;
	static sf::Shader*        flashingShader  ;
	static sf::Shader*        williamsShader  ;
	static sf::Shader*        deathShader;
	static Color<float>       brightColors[]  ;
	static sf::Font*          otherFont       ;
	static HSV                shiftReplacement;
	static sf::RenderTexture* miniTarget      ;
	static sf::Sprite*        miniSprite      ;
	static DeathReplacement deathReplacement;
};