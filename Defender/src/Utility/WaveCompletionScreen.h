#pragma once
#include <cstdint>
#include <SFML/Graphics/Sprite.hpp>

#include "../Entity/Entity.h"
#include "../Utility/DisplayManager.h"

class WaveCompletionScreen : public sf::Drawable
{
public:
	WaveCompletionScreen(uint64_t &score_, uint32_t wave_, uint8_t live_) :
        score(score_), wave(wave_), liveAstronauts(live_), astronautCount{0},
        astronaut(sf::Sprite(*DisplayManager::getTexture(), Entity::getBounds(EntityID::ASTRONAUT)))
	{
		sf::Color textColor = /*sf::Color(255,255,255);*/ UserInterface::getColor();

		//4
		waveText = sf::Text("ATTACK WAVE " + std::to_string(wave), UserInterface::getFont(), 16);
		// 6
		completed = sf::Text("COMPLETED", UserInterface::getFont(), 16);
		// 12
		bonusText = sf::Text("BONUS X " + std::to_string(std::min<uint16_t>(wave, 5) * 100), UserInterface::getFont(), 16);

		waveText .setPosition(DisplayManager::getView().getCenter().x -  waveText.getGlobalBounds().width / 2, COMN::uiHeight + height * 4 );
		completed.setPosition(DisplayManager::getView().getCenter().x - completed.getGlobalBounds().width / 2, COMN::uiHeight + height * 6 );
		bonusText.setPosition(DisplayManager::getView().getCenter().x - bonusText.getGlobalBounds().width / 2, COMN::uiHeight + height * 12);

		astronautTimer.tick(0);
		screenTimer.tick(0);

		waveText.setFillColor(textColor);
		completed.setFillColor(textColor);
		bonusText.setFillColor(textColor);
	}

	bool tick(double deltatime)
	{
		std::cout << "WS tick: " << deltatime << '\n';
		if (astronautCount < liveAstronauts)
		{
			if (astronautTimer.tick(deltatime))
			{
				++astronautCount;
				score += std::min<uint16_t>(wave, 5) * 100;
			}
		}
		else if (screenTimer.tick(deltatime))
			return true;

		return false;
	}
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		// draw 3 texts of the color passed
		target.draw(waveText, states);
		target.draw(completed, states);
		target.draw(bonusText, states);

		//target.draw(astronaut, states);
		// Loop from 0 < astronautCount
		  // draw the astronaut sprite at the appropriate location; +8px?

		astronaut.setPosition(bonusText.getPosition().x - 1, COMN::uiHeight + height * 15);

		for  (uint8_t i = 0; i < astronautCount; i++)
		{
			astronaut.setPosition(bonusText.getPosition().x - 1 + i * 9, astronaut.getPosition().y);
			target.draw(astronaut, states);
		}
	}


private:
	uint32_t wave;
	uint64_t& score;
	const uint8_t liveAstronauts;
	uint8_t astronautCount;
	static constexpr uint8_t height = 8;
	mutable sf::Sprite astronaut;
	sf::Text waveText, completed, bonusText;

	sf::Color textColor;

	Timer<double> astronautTimer{ 0.1 };
	Timer<double> screenTimer{ 3 };
};
