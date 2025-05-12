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


	tick(0);
}

HighscoreState::~HighscoreState()
{
	scoreTxt.setString("");
}

bool HighscoreState::tick(double deltatime)
{
	static Timer<double> timeout{ 8, true }; // 10 seconds
	if (timeout.tick(deltatime))
		return !true;

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
	hallOfFame.setPosition(COMN::resolution.x / 2 - hallOfFame.getGlobalBounds().getSize().x / 2, 50);
	hallOfFame.setFillColor(sf::Color(COMN::ShaderTarget));

	goatodayTitle.setFont(UserInterface::getFont());
	goatodayTitle.setString(" TODAYS \nGREATEST");
	goatodayTitle.setCharacterSize(16);
	goatodayTitle.setPosition(COMN::resolution.x / 4 - goatodayTitle.getGlobalBounds().getSize().x / 2, 80);
	goatodayTitle.setFillColor(sf::Color(COMN::ShaderTarget));

	goatimeTitle.setFont(UserInterface::getFont());
	goatimeTitle.setString("ALL TIME\nGREATEST");
	goatimeTitle.setCharacterSize(16);
	goatimeTitle.setPosition(COMN::resolution.x * 3 / 4 - goatimeTitle.getGlobalBounds().getSize().x / 2, 80);
	goatimeTitle.setFillColor(sf::Color(COMN::ShaderTarget));

	goatodayUnder.setSize(sf::Vector2f(goatodayTitle.getGlobalBounds().getSize().x, 2));
	goatodayUnder.setPosition(COMN::resolution.x / 4 - goatodayUnder.getGlobalBounds().getSize().x / 2, 110);
	goatodayUnder.setFillColor(sf::Color(COMN::ShaderTarget));

	goatimeUnder.setSize(sf::Vector2f(goatimeTitle.getGlobalBounds().getSize().x, 2));
	goatimeUnder.setPosition(COMN::resolution.x * 3 / 4 - goatimeUnder.getGlobalBounds().getSize().x / 2, 110);
	goatimeUnder.setFillColor(sf::Color(COMN::ShaderTarget));

	goatoday.setFont(UserInterface::getFont());
	goatoday.setCharacterSize(16);
	goatoday.setFillColor(sf::Color(COMN::ShaderTarget));

	goatime.setFont(UserInterface::getFont());
	goatime.setCharacterSize(16);
	goatime.setFillColor(sf::Color(COMN::ShaderTarget));

	scoreTxt.setFont(UserInterface::getFont());
	scoreTxt.setCharacterSize(16);
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

	if (score > 0)
	{
		scoreTxt.setString(std::to_string(score));
		scoreTxt.setOrigin(scoreTxt.getGlobalBounds().width, 0);
		scoreTxt.setPosition(63, 14);
	}

}

bool HighscoreState::isHighscore(int score)
{
	return (score > today[7].score || score > allTime[7].score);
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


void HighscoreState::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	UserInterface::getShader(ShaderID::HUE_SHIFT)->setUniform("texture", sf::Shader::CurrentTexture);

	shifting->draw(scoreTxt, states);

	shifting->draw(defender, states);

	shifting->draw(hallOfFame, states);

	shifting->draw(goatodayTitle, states);
	shifting->draw(goatimeTitle, states);

	shifting->draw(goatodayUnder, states);
	shifting->draw(goatimeUnder, states);

	for (int i = 0; i < 8; i++)
	{
		// need to treat string as 3 strings
		// why no monospace

		// today
		goatoday.setString(std::to_string(i));
		goatoday.setPosition(COMN::resolution.x / 4 - 
			goatodayUnder.getGlobalBounds().getSize().x / 2 +
			8 * (-0.75f)
			, 110 + i * 10);
		shifting->draw(goatoday);

		// no i can't even draw the initials as a string
		// they need to be drawn char by char to make sure they are aligned
		for (int ii = 0; ii < 3; ii++)
		{
			goatoday.setString(today[i].initials[ii]);
			goatoday.setPosition(COMN::resolution.x / 4 - 
				goatodayUnder.getGlobalBounds().getSize().x / 2 +
				8 * (ii + 0.5f)
				, 110 + i * 10);
			shifting->draw(goatoday);
		}

		if (today[i].score > 99999)
			today[i].score = 99999;
		std::string score = std::to_string(today[i].score);
		goatoday.setString(score);
		// right align
		goatoday.setPosition(COMN::resolution.x / 4 -
			goatodayUnder.getGlobalBounds().getSize().x / 2 +
			8 * (8.75f - score.length())
			, 110 + i * 10);
		shifting->draw(goatoday);



		// all time
		goatime.setString(std::to_string(i));
		goatime.setPosition(COMN::resolution.x * 3 / 4 -
			goatimeUnder.getGlobalBounds().getSize().x / 2 +
			8 * (-0.75f)
			, 110 + i * 10);
		shifting->draw(goatime);

		for (int ii = 0; ii < 3; ii++)
		{
			goatime.setString(allTime[i].initials[ii]);
			goatime.setPosition(COMN::resolution.x * 3 / 4 -
				goatimeUnder.getGlobalBounds().getSize().x / 2 +
				8 * (ii + 0.5f)
				, 110 + i * 10);
			shifting->draw(goatime);
		}

		if (allTime[i].score > 999999)
			allTime[i].score = 999999;
		score = std::to_string(allTime[i].score);
		goatime.setString(score);
		// right align
		goatime.setPosition(COMN::resolution.x * 3 / 4 -
			goatimeUnder.getGlobalBounds().getSize().x / 2 +
			8 * (8.75 - score.length())
			, 110 + i * 10);
		shifting->draw(goatime);
	}

	states.shader = UserInterface::getShader(ShaderID::HUE_SHIFT);
	target.draw(shftDra, states);
}