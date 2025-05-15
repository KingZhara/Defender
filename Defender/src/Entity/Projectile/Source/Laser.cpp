#include "../Laser.h"

Laser::
Laser(sf::Vector2f pos_, bool isScripted_, EntityScript *script_): Projectile(pos_, EntityID::LASER, isScripted_, script_)
{
    laserTex.create(COMN::resolution.x, 1);
    laserTrail.create(COMN::resolution.x, 1, sf::Color::Transparent);
    laserTex.setRepeated(false);
    shftDra.setTexture(laserTex);

    laserTex.loadFromImage(laserTrail);

    startX = 0;// pos.x;

    if (dynamic_cast<Player*>(EntityData::PLAYER_REF.entity)->getDir())
        shftDra.setScale(-1, 1);

    // @todo V
    // Add a flag to the entity constructor to not make a visual
    // Make collision work for projectiles without a visual
}

void Laser::tick(double deltatime) {
    static auto clamp = [](float value) -> uint16_t {
        value = std::clamp(value, 0.0f, COMN::resolution.x - 1.0f);
        return static_cast<uint16_t>(std::round(value));
    };

    pos.x            = 0;
    float oldDx      = dx;
    bool  playerSign = EntityData::PLAYER_REF.vel->x < 0;

    // Get diff
    Projectile::tick(deltatime);

    // There is an issue with getting the dx when facing left.
    dx += abs(vel.x * deltatime);
    if (dx <= COMN::resolution.x)
    {
        if (laserTrail.getPixel(0, 0) != sf::Color::Black)
            laserTrail.setPixel(0, 0, sf::Color::Black);

        // Solid
        for (int i = 0; i < dx - oldDx + 2; ++i) // + 2 for white tip
            laserTrail.setPixel(clamp(dx - i), 0, sf::Color(136, 0, 255));

        // Noise
        for (int i = 0; i < dx / 5 - oldDx / 5; i++)
            if (rand() % 2 == 0)
                laserTrail.setPixel(clamp(dx - (dx - dx / 5) / 3 - i), 0, sf::Color::Transparent);

        // Transparent
        for (int i = 0; i < dx / 5 - oldDx / 5; i++)
            laserTrail.setPixel(clamp(dx / 5 - i), 0, sf::Color::Black);

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

void Laser::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    UserInterface::getShader(ShaderID::HUE_SHIFT)->setUniform("texture", sf::Shader::CurrentTexture);

    states.shader = UserInterface::getShader(ShaderID::HUE_SHIFT);

    target.draw(shftDra, states);
}
