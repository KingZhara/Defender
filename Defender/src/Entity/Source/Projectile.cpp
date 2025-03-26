#include "../Projectile.h"
#include "../Enemy.h"
#include "../Player.h"

#include "../../Utility/common.h"

bool Projectile::collide(Entity *other)
{
    return ID == EntityID::LASER
               ? dynamic_cast<Enemy*>(other)
                     ? Entity::collide(other)
                     : false
               : dynamic_cast<Player*>(other)
                     ? Entity::collide(other)
                     : false;
}
