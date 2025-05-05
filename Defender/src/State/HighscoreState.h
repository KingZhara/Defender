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

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	static void clean()
	{
		delete defenderTex;
		delete shifting;
	}

	static void initialize();


	static void addScore(const char initials[4], int score);

private:

	static constexpr int HS_COUNT = 8;

	struct Score
	{
		// I made initials 4 long so we can add a null char and use it as a cstring
		char initials[4] = "   ";
		int score = 0;
	};

	static void loadHighscores();

	static void writeHighscores();

	static Score today[HS_COUNT], allTime[HS_COUNT];

	static sf::Text goatoday, goatime, goatodayTitle, goatimeTitle, hallOfFame, scoreTxt;

	static sf::RectangleShape goatodayUnder, goatimeUnder;

	static sf::Texture* defenderTex;
	static sf::RectangleShape defender;

	static sf::RenderTexture* shifting;

	static sf::Sprite shftDra;

};

