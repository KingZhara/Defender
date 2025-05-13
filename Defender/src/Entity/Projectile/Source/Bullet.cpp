#include "../Bullet.h"

Bullet::Bullet(sf::Vector2f pos_, bool isScripted_, EntityScript *script_):
    Projectile(pos_, EntityID::BULLET, isScripted_, script_)
{}

void Bullet::tick(double deltatime) {
    Entity::tick(deltatime);
}
