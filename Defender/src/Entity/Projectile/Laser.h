#pragma once
#include "../Projectile.h"
#include "../Player.h"
class Laser :
    public Projectile
{
public:
    explicit Laser(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr)
        : Projectile(pos_, EntityID::LASER, isScripted_, script_)
    {
        laserTex.create(COMN::resolution.x, 1);
        laserTrail.create(COMN::resolution.x, 1, sf::Color::Transparent);
        laserTex.setRepeated(false);
        shftDra.setTexture(laserTex);

        laserTex.loadFromImage(laserTrail);

        startX = 0;// pos.x;

        if (dynamic_cast<Player*>(EntityData::PLAYER_REF.entity)->getDir())
            shftDra.setScale(-1, 1);

        // Add a flag to the entity constructor to not make a visual
// Make collision work for projectiles without a visual
        //delete visual;
        //visual = nullptr;
    }

    void tick(double deltatime)
    {
        static auto clamp = [](float value) -> uint16_t {
            value = std::clamp(value, 0.0f, COMN::resolution.x - 1.0f);
            return static_cast<uint16_t>(std::round(value));
            };
        // 2px white at front
        // Whole length is divided into 2 sections; 4/5 visible, 1/5 made transparent
        // of the visible length it is made up of 2 other sections; 1/3 solid near the tip, and 2/3 filled with noise
        // the noise stays the same and is slowly covered up as the transparent region expands
        // the noise is revealed over time as the projectile moves
        // We should set the projectiles position as dx from tick of just projectile velocity
        // we can assign a pos.x of 0, run projectile tick, and then update the projectiles pos based on the known previous dx from player pos
        // pos.x = 0
        // projectile tick
        // dx += pos.x
        // for (int i = 0; i < dx / 5.; i++)
        //      trail.setPixel(i, sf::color::transparent)
        // for (int i = 0; i < pos.x; i++) // pos.x is the difference of this tick
        //      trail.setPixel(dx - i, sf::color(shaderColor))
        // //
        //  replace the start of the noise region... the length should be... new lengths noise start to the old lengths noise start... we should only need to set transparent pixels?.
        // //.
        // pos.x = player.pos.x + copysign(dx, vel.x).



        //vel.x = std::copysign(getEVel(ID).x + EntityData::PLAYER_REF.vel->x, vel.x);
        pos.x = 0;
        float oldDx = dx;
        bool playerSign = EntityData::PLAYER_REF.vel->x < 0;

        // update textures here
        // Sorta magic numbers sorry
        // The head moves 2x faster than the tail


        Projectile::tick(deltatime);

        // There is an issue with getting the dx when facing left.
        dx += abs(vel.x * deltatime);
        if (dx <= COMN::resolution.x)
        {
            if (laserTrail.getPixel(0, 0) != sf::Color::Transparent)
                laserTrail.setPixel(0, 0, sf::Color::Transparent);

            // Solid
            for (int i = 0; i < dx - oldDx + 2; ++i) // + 2 for white tip
                laserTrail.setPixel(clamp(dx - i), 0, sf::Color(136, 0, 255));

            // Noise
            for (int i = 0; i < dx / 5 - oldDx / 5; i++)
                if (rand() % 2 == 0)
                    laserTrail.setPixel(clamp(dx - (dx - dx / 5) / 3 - i), 0, sf::Color::Transparent);

            // Transparent
            for (int i = 0; i < dx / 5 - oldDx / 5; i++)
                laserTrail.setPixel(clamp(dx / 5 - i), 0, sf::Color::Transparent);

            // Laser tip
            laserTrail.setPixel(clamp(dx), 0, sf::Color::White);
            laserTrail.setPixel(clamp(dx - 1), 0, sf::Color::White);

            // Update the texture
            laserTex.update(laserTrail);
            shftDra.setOrigin(dx, 0);
            shftDra.setTextureRect({ 0, 0, (uint8_t)dx, 1 });


        }

        if (EntityData::PLAYER_REF.vel->x != 0)
        if (vel.x < 0)
        {
            if (!playerSign)
                startX -= EntityData::PLAYER_REF.vel->x * deltatime;
        }
        else
        {
            if (playerSign)
                startX -= EntityData::PLAYER_REF.vel->x * deltatime;
        }
        pos.x = std::copysign(dx, vel.x) + startX + EntityData::PLAYER_REF.pos->x;

        shftDra.setPosition(pos);
        visual->setPosition(pos);

    }

    void updateTex()
    {
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

    sf::Texture laserTex;
    sf::Image laserTrail;
    sf::Sprite shftDra;

    float dx;
    float startX;
};

