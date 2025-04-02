#include "../HighscoreState.h"
#include "../../Utility/Timer.h"
#include "../../Utility/common.h"
#include "../../Utility/UserInterface/UserInterface.h"

HighscoreState::HighscoreState()
{
	defenderTex.loadFromFile("res/defender.png");
	defender.setSize(sf::Vector2f(defenderTex.getSize()) / 2.f);
	defender.setTexture(&defenderTex);
	defender.setPosition(COMN::resolution.x / 2 - defender.getGlobalBounds().getSize().x / 2, 30);

	hallOfFame.setFont(UserInterface::getFont());
	hallOfFame.setString("HALL OF FAME");
	hallOfFame.setCharacterSize(16);
	hallOfFame.setPosition(COMN::resolution.x / 2 - (18 * 12.75f / 4), 50);
	hallOfFame.setFillColor(lime);

	goatodayTitle.setFont(UserInterface::getFont());
	goatodayTitle.setString(" TODAYS \nGREATEST");
	goatodayTitle.setCharacterSize(16);
	goatodayTitle.setPosition(COMN::resolution.x / 4 - (18 * 8.f / 4), 80);
	goatodayTitle.setFillColor(lime);

	goatimeTitle.setFont(UserInterface::getFont());
	goatimeTitle.setString("ALL TIME\nGREATEST");
	goatimeTitle.setCharacterSize(16);
	goatimeTitle.setPosition(COMN::resolution.x / 4 * 3 - (18 * 8.f / 4), 80);
	goatimeTitle.setFillColor(lime);

	goatodayUnder.setSize(sf::Vector2f(18 * 8.5f / 2, 2));
	goatodayUnder.setPosition(COMN::resolution.x / 4 - (18 * 8.f / 4), 110);
	goatodayUnder.setFillColor(lime);

	goatimeUnder.setSize(sf::Vector2f(18 * 8.5f / 2, 2));
	goatimeUnder.setPosition(COMN::resolution.x / 4 * 3 - (18 * 8.f / 4), 110);
	goatimeUnder.setFillColor(lime);

	goatoday.setFont(UserInterface::getFont());
	goatoday.setString(makeScores(today));
	goatoday.setCharacterSize(16);
	goatoday.setPosition(COMN::resolution.x / 4 - (18 * 10.f / 4), 110);
	goatoday.setFillColor(lime);

	goatime.setFont(UserInterface::getFont());
	goatime.setString(makeScores(allTime));
	goatime.setCharacterSize(16);
	goatime.setPosition(COMN::resolution.x / 4 * 3 - (18 * 10.f / 4), 110);
	goatime.setFillColor(lime);

	tick(0);
}

HighscoreState::~HighscoreState()
{
}

bool HighscoreState::tick(double deltatime)
{
	static Timer<double> timeout{ 8 }; // 10 seconds

	if (timeout.tick(deltatime))
		return true;

	return false;
}

std::string HighscoreState::padNum(int num, int length)
{
	std::string str(length, 0);
	snprintf(str.data(), length + 1, "%*d", length, num); // This is a c++ class.
	return str;
}

std::string HighscoreState::makeScores(Score scores[8])
{
	std::string str;
	for (int i = 0; i < 8; i++)
	{
		str += ('1' + i);
		str += scores[i].initials[0];
		str += scores[i].initials[1];
		str += scores[i].initials[2];
		str += padNum(scores[i].score, 6);
		str += '\n';
	}
	return str;
}
