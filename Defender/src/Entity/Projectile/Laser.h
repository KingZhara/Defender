#pragma once
#include "../Projectile.h"
class Laser :
    public Projectile
{
public:
    explicit Laser(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr)
        : Projectile(pos_, EntityID::LASER, isScripted_, script_)
    {
        shifting.create(COMN::resolution.x, 1);
        laser.create(COMN::resolution.x, 1);
        shftDra.setTexture(shifting);

        // Laser beam randomness doesn't move with the laser
        for (int x = 0; x < laser.getSize().x; x++)
        {
            if (rand() % 2)
                laser.setPixel(x, 0, sf::Color(136, 0, 255));
        }
    }

    void tick(double deltatime)
    {
        // update textures here

        shifting.update(laser);

        shftDra.setPosition(getPos());


        Entity::tick(deltatime);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        // Laser has 2px white at front
        // The rest shifts rainbow all at once
        // 
        // Laser has first 1/3 solid
        // 
        // Laser has back  2/3 random
        // The random on/off stays the same,
        // They don't move with the laser
        // 
        // Laser head moves twice as fast as laser tail
        // Laser disapears when the head touches the screen edge

        UserInterface::getShader(ShaderID::HUE_SHIFT)->setUniform("texture", sf::Shader::CurrentTexture);

        states.shader = UserInterface::getShader(ShaderID::HUE_SHIFT);
        target.draw(shftDra, states);
    }

private:

    sf::Texture shifting;
    sf::Image laser;
    sf::Sprite shftDra;

    float start = 0, end = 0;
};

