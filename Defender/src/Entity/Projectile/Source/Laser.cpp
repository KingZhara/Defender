#include "../Laser.h"
bool Laser::collide(Entity *other)
{
    auto bound = sf::FloatRect(pos, { 1, 1 });
    return other->collide(bound);
}
