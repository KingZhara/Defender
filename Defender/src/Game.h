#pragma once

/*
	Game runs most of the program
	Game holds a GameState, which holds the actual game,
	and the attract and highscore screens.

	GameState switches the state screens and updates them

	StageState updates the actual game
*/

#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Utility/Action.h"

class Game : public sf::Drawable
{
public:

	Game();

	~Game();

	void tick(Action& actions, double deltatime);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:

	GameState state;
};
/*
purple : #80F
Magenta: #F0F
blue:  : #080
green  : #080
yellow : #ff0
cyan   : #0ff
red    : #f00
gray   : #888
white  : #fff
black  : #000
brown  : #940
*/
