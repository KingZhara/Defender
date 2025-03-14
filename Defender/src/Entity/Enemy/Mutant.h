#pragma once
#include "../Enemy.h"
class Mutant :
    public Enemy
{
    Mutant(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr)
        : Enemy(pos_, EntityID::MUTANT, isScripted_, script_)
    {}

    /*
    The mutant gives the player 150 xp upon death. Mutants just fly 
    horizontally until the player gets within range, then they fly directly 
    toward the player and shoots at them , always going horizontally before 
    they move vertically.
    */
    uint8_t attack = 0;
    uint16_t attackPos;


    //https://en.wikipedia.org/wiki/List_of_software_palettes#/media/File:RGB_6levels_palette_color_test_chart.png
private:

    void tick(double deltaTime);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

