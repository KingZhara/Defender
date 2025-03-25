#pragma once
#include "../Enemy.h"

class Mutant :
    public Enemy
{
public:
    explicit Mutant(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr)
        : Enemy(pos_, EntityID::MUTANT, isScripted_, script_)
    {
    }

    /*
    The mutant gives the player 150 xp upon death. Mutants just fly 
    horizontally until the player gets within range, then they fly directly 
    toward the player and shoots at them , always going horizontally before 
    they move vertically.
    */

    //https://en.wikipedia.org/wiki/List_of_software_palettes#/media/File:RGB_6levels_palette_color_test_chart.png

    virtual void tick(double deltatime) override;

    static void initShader();


private:
    // Range [0, 3]; 0, 1 = up; 2, 3 = down;
    uint8_t bobStage;
    Timer<double> bobbing{ 1. / 15. };
    static sf::Shader* shader;

    static std::vector<sf::Glsl::Vec3> brightColors;
};

