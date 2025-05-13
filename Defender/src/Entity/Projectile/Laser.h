#pragma once
#include "../Projectile.h"
#include "../Player.h"
class Laser :
    public Projectile
{
public:
    explicit Laser(sf::Vector2f pos_,
        bool         isScripted_ = false, EntityScript* script_ = nullptr);

    virtual ~Laser() override = default;

    virtual void tick(double deltatime) override;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::Texture laserTex;
    sf::Image laserTrail;
    sf::Sprite shftDra;

    float dx = 0;
    float startX;
};

