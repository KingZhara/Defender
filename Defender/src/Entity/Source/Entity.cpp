#include "../Entity.h"

#include "../../Utility/DisplayManager.h"

std::queue<Entity::QueuedEntity> Entity::entityQueue;

sf::Vector2f *Entity::EntityData::PlayerRef::pos = nullptr;
sf::Vector2f* Entity::EntityData::PlayerRef::vel = nullptr;
Entity* Entity::EntityData::PlayerRef::entity = nullptr;

sf::Sprite *Entity::miniSprite = nullptr;

const Entity::EntityData Entity::DATA_TABLE[EntityID::LENGTH] =
{
    // Player
    EntityData{
        // Velocity factor
        Vec2<double>{
            1.5f,
            1.5f
        },
        // XP
        0
    },
    // Astronaut
    EntityData{
        Vec2<double>{
            1.0f,
            1.0f
        },
        250
    },
    // Bullet
    EntityData{
        Vec2<double>{
            1.0f,
            1
        },
        0
    },
    // Laser
    // @todo Replace later; blank data, will not use a sprite for drawing, custom image
    EntityData{
        Vec2<double>{
            1.0f,
            1.0f
        },
        0
    },
    // Bomb
    EntityData{
        Vec2<double>{
            1.0f,
            1.0f
        },
        0
    },
    // Lander
    EntityData{
        Vec2<double>{
            .35f,
            .35f
        },
        150
    },
    // Mutant
    EntityData{
        Vec2<double>{
            .75f,
            .75f
        },
        150
    },
    // Baiter
    EntityData{
        Vec2<double>{
            0.5,
            0.5
        },
        200
    },
    // Bomber
    EntityData{
        Vec2<double>{
            0,
            0
        },
        250
    },
    // Pod
    EntityData{
        Vec2<double>{
            1.0f,
            1.0f
        },
        1000
    },
    // Swarmer
    EntityData{
        Vec2<double>{
            1,
            1
        },
        150
    },
    // Particle
    EntityData{
        Vec2<double>{
            1.f,
            1.f,
        },
        0

    },
    // Piece
    EntityData{
        Vec2<double>{
            1.0f,
            1.0f
        },
        150
    },
    // Death animation piece
    EntityData{
        Vec2<double>{
            1.0f,
            1.0f
        },
        150
    },
};

void Entity::tick(double deltatime)
{
    pos += {
        std::round(static_cast<float>(vel.x * deltatime)),
        std::round(static_cast<float>(vel.y * deltatime))
    };

    if (abs(vel.x) < 0.1f)
        vel.x = 0;
    if (abs(vel.y) < 0.1f)
        vel.y = 0;

    //visual->printDebugInfo();
    wrap(); // Overridden in player

    visual->setPosition(pos);
    //std::cout << '\n';
}

bool Entity::collide(Entity *other)
{
    return visual->intersects(other->visual);
}

void Entity::applyFriction(double deltatime, double time)
{
    if (time == 0)
        throw std::runtime_error("DIVISION BY 0 FRICTION");

    sf::Vector2f fric = getEVel(ID);
    fric.x /= time;
    fric.y /= time;

    fric.x *= deltatime;
    fric.y *= deltatime;

    fric.x = std::min(abs(vel.x), fric.x);
    fric.y = std::min(abs(vel.y), fric.y);

    if (vel.x < 0)
        fric.x = -fric.x;
    if (vel.y < 0)
        fric.y = -fric.y;

    vel -= fric;
}

bool Entity::collide(sf::FloatRect otherBound)
{
    return visual->intersects(otherBound);
}

void Entity::wrap()
{
    if (pos.x < 0)
        pos.x += COMN::worldSize;
    else if (pos.x > COMN::worldSize)
        pos.x -= COMN::worldSize;
}

void Entity::setPos(sf::Vector2f newPos)
{
    pos = newPos;
    visual->setPosition(newPos);
}

void Entity::setVel(sf::Vector2f newVel) { vel = newVel; }

const Entity::EntityTarget Entity::makePlayerTargetedVec(
        sf::Vector2f pos,
        EntityID::ID ID,
        uint8_t      scale,
        bool         playerVelType)
{
    return makeTargetedVec(pos, ID, EntityData::PLAYER_REF.entity, scale, playerVelType);
}

const sf::IntRect Entity::getBounds(EntityID::ID ID)
{
    return VisualComponent::getBounds(ID);
}

const sf::Vector2f Entity::makeCenteredTL(sf::Vector2f pos, EntityID::ID id)
{
    return {
        pos.x + static_cast<float>(VisualComponent::getBounds(id).width /
            2.),
        pos.y + static_cast<float>(VisualComponent::getBounds(id).height /
            2.)
    };
}

const sf::Vector2f Entity::getEVel(EntityID::ID ID_)
{
    sf::Vector2f ret = {
        static_cast<float>(EntityData::BASE_VELOCITY.x),
        static_cast<float>(EntityData::BASE_VELOCITY.y)
    };


    ret.x *= static_cast<float>(DATA_TABLE[ID_].VELOCITY_FACTOR.x);
    ret.y *= static_cast<float>(DATA_TABLE[ID_].VELOCITY_FACTOR.y);

    return ret;
}

bool Entity::isOnScreen()
{
    static constexpr float half = COMN::resolution.x / 2;
    float left = DisplayManager::getView().getCenter().x - half,
          right = DisplayManager::getView().getCenter().x + half;
    return (pos.y >= 0 && pos.y <= COMN::resolution.y) &&
    (
        (pos.x >= left - COMN::worldSize && pos.x <= right - COMN::worldSize) ||
        (pos.x >= left && pos.x <= right) ||
        (pos.x >= left + COMN::worldSize && pos.x <= right + COMN::worldSize)
    );
}

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::Vector2f bound = {
        DisplayManager::getView().getCenter().x - DisplayManager::getView().
                                                  getSize().x / 2 -
        COMN::worldSize,
        DisplayManager::getView().getCenter().x + DisplayManager::getView().
                                                  getSize().x / 2 -
        COMN::worldSize
    };

    float specialX = 0;
    if (pos.x < 0 + bound.y)
        specialX = pos.x + COMN::worldSize;
    else if (pos.x + VisualComponent::getBounds(ID).width > COMN::worldSize +
        bound.x)
        specialX = pos.x - COMN::worldSize;

    // Handle the miniSprite
    if ((ID < EntityID::BULLET || ID > EntityID::BOMB) && ID != EntityID::PIECE && ID != EntityID::DEATH_ANIM_PIECE)
    {
        miniSprite->setPosition(makeMiniDrawPos());
        UserInterface::getMiniTarget()->draw(*miniSprite, states);
    }

    if (ID != EntityID::PARTICLE)
    {
        // Handle the main sprite
        target.draw(*visual, states);
        if (specialX != 0)
        {
            visual->setPosition({specialX, pos.y});
            target.draw(*visual, states);
        }
    }
}

sf::Vector2f Entity::makeMiniDrawPos() const
{
    sf::Vector2f pos_;

    pos_.x = std::round(pos.x * UserInterface::UIBounds::minimapPosConversion.x);
    pos_.y = std::round(pos.y * UserInterface::UIBounds::minimapPosConversion.y);

    return pos_;
}

const Entity::EntityTarget Entity::makeTargetedVec(sf::Vector2f pos, EntityID::ID ID, Entity* other,
	uint8_t scale, bool velType)
{
    static constexpr float half = COMN::worldSize / 2;

    sf::Vector2f target = makeCenteredTL({ 0, 0 }, other->getID());
    sf::Vector2f eVel = getEVel(ID);

    if (velType)
        target += other->getVel();

    pos = makeCenteredTL(pos, ID);
    // Use the center & normalize
    pos.x -= half;

    // Normalize the target
    target.x += other->getPos().x - pos.x - half;
    target.y += other->getPos().y - pos.y;

    // Apply wrapping
    if (target.x > half)
        target.x -= COMN::worldSize;
    else if (target.x < -half)
        target.x += COMN::worldSize;

    // Generate the angle
    double rot = atan2(target.y, target.x);

    // Generate the velocity
    sf::Vector2f vel = {
        static_cast<float>(cos(rot) * scale * eVel.x),
        static_cast<float>(sin(rot) * scale * eVel.y)
    };

    if (!velType)
        vel += other->getVel();


    return { vel, target };
}
