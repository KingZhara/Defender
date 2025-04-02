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

private:

	struct Score
	{
		char initials[3] = { 'A', 'B', 'C' };
		int score = 12345;
	};

	std::string padNum(int num, int length);

	std::string makeScores(Score scores[8]);

	Score today[8], allTime[8];

	sf::Text goatoday, goatime, goatodayTitle, goatimeTitle, hallOfFame;

	sf::RectangleShape goatodayUnder, goatimeUnder;

	sf::Texture defenderTex;
	sf::RectangleShape defender;

	sf::Color lime = sf::Color(0x89FF00FF);

	sf::RenderTexture* shifting;

	sf::Sprite flshDra, shftDra;
};

