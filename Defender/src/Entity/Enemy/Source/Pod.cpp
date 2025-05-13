#include "../Pod.h"

Pod::Pod(sf::Vector2f pos_, bool isScripted_, EntityScript *script_): Enemy(pos_, EntityID::POD, isScripted_, script_)
{
    rot = ((rand() % 120) - 60) + (180 * (rand() % 2));

    vel.x = (float)(43 * cos(rot * (3.141592653589 / 180)));
    vel.y = (float)(43 * sin(rot * (3.141592653589 / 180)));
}

Pod::~Pod() {
    std::cout << "Pod destroyed" << std::endl;
}

void Pod::tick(double deltatime) {
    std::cout << "Pod tick" << std::endl;

    Enemy::tick(deltatime);
}
