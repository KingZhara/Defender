#include "../Bomb.h"

Bomb::Bomb(sf::Vector2f pos_, bool isScripted_, EntityScript *script_): Projectile(pos_, EntityID::SWARMER, isScripted_, script_)
{}

void Bomb::tick(double deltatime) {
    applyFriction(deltatime, 3);
}
