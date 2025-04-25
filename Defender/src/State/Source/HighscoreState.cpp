#include "../HighscoreState.h"
#include "../../Utility/Timer.h"
#include "../../Utility/common.h"

#include <fstream>

constexpr int HS_COUNT = 8;

HighscoreState::Score HighscoreState::today[HS_COUNT], HighscoreState::allTime[HS_COUNT];

sf::Text HighscoreState::goatoday, HighscoreState::goatime, 
HighscoreState::goatodayTitle, HighscoreState::goatimeTitle, 
HighscoreState::hallOfFame, HighscoreState::scoreTxt;

sf::RectangleShape HighscoreState::goatodayUnder, HighscoreState::goatimeUnder;

sf::Texture* HighscoreState::defenderTex;
sf::RectangleShape HighscoreState::defender;

sf::RenderTexture* HighscoreState::shifting;

sf::Sprite HighscoreState::shftDra;


HighscoreState::HighscoreState()
{
	shifting->clear();

	// Call something like this when stagestate is done
	// addScore("TST", 1000);

	goatoday.setString(makeScores(today));
	goatime.setString(makeScores(allTime));

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

void HighscoreState::initialize()
{
	defenderTex = new sf::Texture;

	defenderTex->loadFromFile("res/defender.png");
	defender.setSize(sf::Vector2f(defenderTex->getSize()) / 2.f);
	defender.setTexture(defenderTex);
	defender.setPosition(COMN::resolution.x / 2 - defender.getGlobalBounds().getSize().x / 2, 30);

	hallOfFame.setFont(UserInterface::getFont());
	hallOfFame.setString("HALL OF FAME");
	hallOfFame.setCharacterSize(16);
	hallOfFame.setPosition(COMN::resolution.x / 2 - (18 * 12.75f / 4), 50);
	hallOfFame.setFillColor(sf::Color(COMN::ShaderTarget));

	goatodayTitle.setFont(UserInterface::getFont());
	goatodayTitle.setString(" TODAYS \nGREATEST");
	goatodayTitle.setCharacterSize(16);
	goatodayTitle.setPosition(COMN::resolution.x / 4 - (18 * 8.f / 4), 80);
	goatodayTitle.setFillColor(sf::Color(COMN::ShaderTarget));

	goatimeTitle.setFont(UserInterface::getFont());
	goatimeTitle.setString("ALL TIME\nGREATEST");
	goatimeTitle.setCharacterSize(16);
	goatimeTitle.setPosition(COMN::resolution.x / 4 * 3 - (18 * 8.f / 4), 80);
	goatimeTitle.setFillColor(sf::Color(COMN::ShaderTarget));

	goatodayUnder.setSize(sf::Vector2f(18 * 8.5f / 2, 2));
	goatodayUnder.setPosition(COMN::resolution.x / 4 - (18 * 8.f / 4), 110);
	goatodayUnder.setFillColor(sf::Color(COMN::ShaderTarget));

	goatimeUnder.setSize(sf::Vector2f(18 * 8.5f / 2, 2));
	goatimeUnder.setPosition(COMN::resolution.x / 4 * 3 - (18 * 8.f / 4), 110);
	goatimeUnder.setFillColor(sf::Color(COMN::ShaderTarget));

	goatoday.setFont(UserInterface::getFont());
	goatoday.setCharacterSize(16);
	goatoday.setPosition(COMN::resolution.x * 0.25f - (18 * 10.f / 4), 110);
	goatoday.setFillColor(sf::Color(COMN::ShaderTarget));

	goatime.setFont(UserInterface::getFont());
	goatime.setCharacterSize(16);
	goatime.setPosition(COMN::resolution.x * 0.75f - (18 * 10.f / 4), 110);
	goatime.setFillColor(sf::Color(COMN::ShaderTarget));

	scoreTxt.setFont(UserInterface::getFont());
	scoreTxt.setCharacterSize(16);
	scoreTxt.setPosition(COMN::resolution.x * 0.25f - (18 * 10.f / 4), 10);
	scoreTxt.setFillColor(sf::Color(COMN::ShaderTarget));

	shifting = new sf::RenderTexture;
	shifting->create((unsigned)COMN::resolution.x, (unsigned)COMN::resolution.y);
	shifting->setView(DisplayManager::getView());
	shftDra.setTexture(shifting->getTexture());
	shftDra.setScale(1.f, -1.f);
	shftDra.move(0, COMN::resolution.y);

	loadHighscores();
}

void HighscoreState::addScore(const char initials[4], int score)
{
	for (int i = 0; i < HS_COUNT; i++)
	{
		if (score > today[i].score)
		{
			for (int ii = HS_COUNT - 1; ii > i; ii--)
				today[ii] = today[ii - 1];
			today[i].score = score;
			for (int c = 0; c < 4; c++)
				today[i].initials[c] = initials[c];
			break;
		}
	}
	// looks like this could be just one loop,
	// but it does need to break out individually.
	// That was the weird score not being counted bug
	for (int i = 0; i < HS_COUNT; i++)
	{
		if (score > allTime[i].score)
		{
			for (int ii = HS_COUNT - 1; ii > i; ii--)
				allTime[ii] = allTime[ii - 1];
			allTime[i].score = score;
			for (int c = 0; c < 4; c++)
				allTime[i].initials[c] = initials[c];

			writeHighscores();
			break;
		}
	}

	scoreTxt.setString(std::to_string(score));
	scoreTxt.setOrigin(scoreTxt.getLocalBounds().width, 0);

}

void HighscoreState::loadHighscores()
{	
	std::ifstream hs("highscores", std::ios::binary);

	if (hs.is_open())
	{
		for (int i = 0; i < HS_COUNT; i++)
		{
			hs.read(allTime[i].initials, sizeof(Score::initials));
			hs.read((char*)&allTime[i].score, sizeof(Score::score));
		}

		hs.close();
	}
	else // highscores not created yet maybe
		writeHighscores();
}

void HighscoreState::writeHighscores()
{
	std::ofstream hs("highscores", std::ios::binary);
	if (hs.is_open())
	{
		for (int i = 0; i < HS_COUNT; i++)
		{
			hs.write(allTime[i].initials, sizeof(Score::initials));
			hs.write((char*)&allTime[i].score, sizeof(Score::score));
		}

		hs.close();
	}
}

std::string HighscoreState::makeScores(Score scores[HS_COUNT])
{
	std::string str(11 * HS_COUNT, ' ');
	for (int i = 0; i < HS_COUNT; i++)
	{
		if (scores[i].score == 0) // NULL score
			snprintf(&str[i * 11], 12, "%d         \n", (i + 1));

		// Explaining this printf format:
		// "Do not be afraid" there is much worse
		//
		// "%d" - prints int
		// "%s" - prints cstring
		// "%6d" - prints padded int (6 digits length)

		// The buffer parameter math is to skip to line i
		// The buffer count being 1 more than the line length is intentional.
		//     It has to write the null char somewhere.
		//     If it's not the final loop it writes to the next line, 
		//     which will be overwritten next, so it doesn't matter.
		//     If it's the final loop it writes it to std::string's null char,
		//     so it is the same and doesn't matter.

		else
			snprintf(&str[i * 11], 12, "%d%s%6d\n", 
				(i + 1), // always gonna be 1 digit
				scores[i].initials, // always 3 characters
				scores[i].score % 100000); // wrap around to keep 5 digits (+ 1 space padding)
	}
	return str;
}
