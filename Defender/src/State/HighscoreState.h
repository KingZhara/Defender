#pragma once

#include <SFML/Graphics.hpp>

class HighscoreState : public sf::Drawable
{
public:

	HighscoreState();

	~HighscoreState();

	bool tick(double deltatime);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(defender, states);

		target.draw(hallOfFame, states);

		target.draw(goatodayTitle, states);
		target.draw(goatimeTitle, states);

		target.draw(goatodayUnder, states);
		target.draw(goatimeUnder, states);

		target.draw(goatoday);
		target.draw(goatime);
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
};

