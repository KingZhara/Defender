#pragma once

#include <SFML/Graphics.hpp>

#include "../Utility/UserInterface/UserInterface.h"
#include "../Utility/DisplayManager.h"

class HighscoreState : public sf::Drawable
{
public:

	HighscoreState();

	~HighscoreState();

	bool tick(double deltatime);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		UserInterface::getShiftingShader()->setUniform("texture", sf::Shader::CurrentTexture);

		shifting->draw(defender, states);

		shifting->draw(hallOfFame, states);

		shifting->draw(goatodayTitle, states);
		shifting->draw(goatimeTitle, states);

		shifting->draw(goatodayUnder, states);
		shifting->draw(goatimeUnder, states);

		shifting->draw(goatoday);
		shifting->draw(goatime);

		states.shader = UserInterface::getShiftingShader();
		target.draw(shftDra, states);
	}

	static void clean()
	{
		delete defenderTex;
		delete shifting;
	}

	static void initialize();

private:

	struct Score
	{
		char initials[3] = { 'A', 'B', 'C' };
		int score = 12345;
	};

	static std::string padNum(int num, int length);

	static std::string makeScores(Score scores[8]);

	static Score today[8], allTime[8];

	static sf::Text goatoday, goatime, goatodayTitle, goatimeTitle, hallOfFame;

	static sf::RectangleShape goatodayUnder, goatimeUnder;

	static sf::Texture* defenderTex;
	static sf::RectangleShape defender;

	static sf::RenderTexture* shifting;

	static sf::Sprite shftDra;
};

