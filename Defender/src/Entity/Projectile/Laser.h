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
        shftDra.setTexture(shifting.getTexture());
        shftDra.setPosition(getPos());

        laserTex.create(COMN::resolution.x, 1);
        laserNoise.create(COMN::resolution.x, 1, sf::Color(0));
        laserTex.setRepeated(true);

        // Laser beam randomness doesn't move with the laser
        for (int x = 0; x < laserNoise.getSize().x; x++)
            if (rand() % 2)
                laserNoise.setPixel(x, 0, sf::Color::White);
        laserTex.loadFromImage(laserNoise);


        head = tail = 0;
    }

    void tick(double deltatime)
    {
        // update textures here

        // Sorta magic numbers sorry
        // The head moves 2x faster than the tail
        head += abs(vel.x) * deltatime * 1.333f;
        tail += abs(vel.x) * deltatime * 0.667f;

        if (vel.x < 0)
        {
            shftDra.setScale(-1, 1);
            shftDra.setPosition(getPos().x + (head - tail), getPos().y + 3);
        }
        else
        {
            shftDra.setPosition(getPos().x - (head - tail), getPos().y + 3);
        }

        Projectile::tick(deltatime);
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

        shifting.clear();

        // Random back 2/3 of laser
        sf::RectangleShape laserRand;
        laserRand.setFillColor(sf::Color(136, 0, 255));
        laserRand.setSize(sf::Vector2f((head - tail) * 0.667f, 1));
        laserRand.setPosition(0, 0);
        laserRand.setTexture(&laserTex);
        // scroll texture back to undo movement
        laserRand.setTextureRect(sf::IntRect(-getPos().x + (head * 0.667f + tail), 
            0, (head - tail) * 0.667f, 1));
        shifting.draw(laserRand, states);


        // Soid 1/3 of laser
        sf::RectangleShape laserSolid;
        laserSolid.setFillColor(sf::Color(136, 0, 255));
        laserSolid.setSize(sf::Vector2f((head - tail) * 0.333f, 1));
        laserSolid.setPosition((head - tail) * 0.667f, 0);
        shifting.draw(laserSolid, states);


        sf::RectangleShape laserHead;
        laserHead.setPosition(head - tail, 0);
        laserHead.setSize({ 2, 1 });
        shifting.draw(laserHead, states);


        states.shader = UserInterface::getShader(ShaderID::HUE_SHIFT);

        target.draw(shftDra, states);
    }

private:

    mutable sf::RenderTexture shifting;
    sf::Texture laserTex;
    sf::Image laserNoise;
    sf::Sprite shftDra;

    float head = 0, tail = 0;
};

