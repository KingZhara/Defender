#include "../HighscoreState.h"
#include "../../Utility/Timer.h"
#include "../../Utility/common.h"

#include <fstream>

constexpr int HS_COUNT = 8;

HighscoreState::Score HighscoreState::today[HS_COUNT], HighscoreState::allTime[HS_COUNT];

sf::Text HighscoreState::goatoday, HighscoreState::goatime, 
HighscoreState::goatodayTitle, HighscoreState::goatimeTitle, 
HighscoreState::hallOfFame;

sf::RectangleShape HighscoreState::goatodayUnder, HighscoreState::goatimeUnder;

sf::Texture* HighscoreState::defenderTex;
sf::RectangleShape HighscoreState::defender;

sf::RenderTexture* HighscoreState::shifting;

sf::Sprite HighscoreState::shftDra;


HighscoreState::HighscoreState()
{
	shifting->clear();

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
	goatoday.setPosition(COMN::resolution.x / 4 - (18 * 10.f / 4), 110);
	goatoday.setFillColor(sf::Color(COMN::ShaderTarget));

	goatime.setFont(UserInterface::getFont());
	goatime.setCharacterSize(16);
	goatime.setPosition(COMN::resolution.x / 4 * 3 - (18 * 10.f / 4), 110);
	goatime.setFillColor(sf::Color(COMN::ShaderTarget));

	shifting = new sf::RenderTexture;
	shifting->create((unsigned)COMN::resolution.x, (unsigned)COMN::resolution.y);
	shifting->setView(DisplayManager::getView());
	shftDra.setTexture(shifting->getTexture());
	shftDra.setScale(1.f, -1.f);
	shftDra.move(0, COMN::resolution.y);

	loadHighscores();
}

void HighscoreState::addScore(char initials[4], int score)
{
}

int HighscoreState::getDate()
{
	time_t now = time(NULL);
	struct tm local = *std::localtime(&now);

	int date = local.tm_year + 1900;		// [0000-9999]
	date += local.tm_mday * 1'00'0000;		// [01-31]
	date += (local.tm_mon + 1) * 1'0000;	// [01-12]
	return date;
}

void HighscoreState::loadHighscores()
{
	/*
		format of "highscores":
		binary

		score format:
			initials score
			(3xchar), (int)

		first section:
			todays date (DDMMYYYY) (int)
		second section:
			8 ordered scores (today) (high first)
			score is -1 and initials are "   " if not set
		third section:
			8 ordered scores (all time) (high first)

		example:

		highscore--------------------------------------------------------------
		03042025
		QWE3000
		RTY2500
		UIO2000
		PAS1500
		___-1
		___-1
		___-1
		___-1
		QWE250000
		RTY200000
		UIO125000
		PAS50000
		DFG25000
		HJK20000
		LZX10000
		CVB5000
		end highscore----------------------------------------------------------

		highscore binary-------------------------------------------------------
		Date:        | 0x002E6AE9
		1st today:   | 0x51 0x57 0x45 0x00 0x00000BB8
		2nd today:   | 0x52 0x54 0x59 0x00 0x000009C4
		3rd today:   | 0x20 0x20 0x20 0x00 0xFFFFFFFF
		4th today:   | 0x20 0x20 0x20 0x00 0xFFFFFFFF
		5th today:   | 0x20 0x20 0x20 0x00 0xFFFFFFFF
		6th today:   | 0x20 0x20 0x20 0x00 0xFFFFFFFF
		7th today:   | 0x20 0x20 0x20 0x00 0xFFFFFFFF
		8th today:   | 0x20 0x20 0x20 0x00 0xFFFFFFFF
		1st all time:| 0x51 0x57 0x45 0x00 0x0003D090
		2nd all time:| 0x51 0x57 0x45 0x00 0x0003D090
		3rd all time:| 0x51 0x57 0x45 0x00 0x0003D090
		4th all time:| 0x51 0x57 0x45 0x00 0x0003D090
		5th all time:| 0x51 0x57 0x45 0x00 0x0003D090
		6th all time:| 0x51 0x57 0x45 0x00 0x0003D090
		7th all time:| 0x51 0x57 0x45 0x00 0x0003D090
		8th all time:| 0x51 0x57 0x45 0x00 0x0003D090
		end highscore binary---------------------------------------------------
	*/
	
	std::ifstream hs("highscores", std::ios::binary);

	if (hs.is_open())
	{
		int date = getDate();
		hs.read((char*)&date, sizeof(int));

		for (int i = 0; i < HS_COUNT; i++)
		{
			hs.read(today[i].initials, sizeof(Score::initials));
			hs.read((char*)&today[i].score, sizeof(Score::score));
		}
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
		int date = getDate();
		hs.write((char*)&date, sizeof(int));

		for (int i = 0; i < HS_COUNT; i++)
		{
			hs.write(today[i].initials, sizeof(Score::initials));
			hs.write((char*)&today[i].score, sizeof(Score::score));
		}
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
	std::string str(11 * 8, 0);
	for (int i = 0; i < HS_COUNT; i++)
	{
		if (scores[i].score == -1) // NULL score
			break;

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

		snprintf(&str[i * 11], 12, "%d%s%6d\n", 
			(i + 1), // always gonna be 1 digit
			scores[i].initials, // always 3 characters
			scores[i].score % 1000000); // wrap around to keep 6 digits
	}
	return str;
}
