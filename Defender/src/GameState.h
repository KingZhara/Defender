#pragma once

#include <SFML/Graphics.hpp>
#include "State/State.hpp"
#include "Utility/Action.h"
#include <variant>

/**
 * 
 */
struct GameState : sf::Drawable
{
	/**
	 * The different Game states
	 */
	enum class State : uint8_t
	{
		ATTRACT,
		HIGHSCORE,
		STAGE,
		SCANNER
	};

	/**
	 * Constructor; initializes as AttractState
	 */
	GameState();

	/**
	 * Destructor; Maps to the appropriate state destructor
	 */
	~GameState() override = default;

	/**
	 * Maps to the appropriate state tick method
	 * 
	 * @return If state must change
	 */
	bool tick(Action& actions, double deltatime);

	/**
	 * Maps to the appropriate draw method with the supplied data
	 * 
	 * @param target The render target; what is being drawn / drawn to
	 * @param states The render states used when drawing the stages
	 */
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	/**
	 * Switches the state according to the default rules:
	 * 1. Attract   -> Highscore
	 * 2. Highscore -> Scanner
	 * 2. Scanner   -> Attract
	 * 3. Stage     -> Highscore
	 * 4. *         -> Stage (stage == true?)
	 * 
	 * @param stage Stage switch predicate
	 */
	void switchState(bool stage = false);

	// The current state
	State type;
	std::variant<AttractState, HighscoreState, StageState, ScannerState> state = AttractState();
};
