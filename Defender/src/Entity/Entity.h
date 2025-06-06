#pragma once
#include <iostream>
#include <queue>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "../Utility/VisualComponent/Animation.h"
#include "../Utility/EntityID.h"
#include "../Utility/EntityScript/EntityScript.h"
#include "../Utility/VisualComponent/VisualComponent.h"
#include "../Utility/VisualComponent/PieceVisualComponent.h"

// new EntityScript(EntityScript::ScriptType::DONE) - BLANK SCRIPT

class Entity : public sf::Drawable
{
public:
    struct EntityTarget
    {
        sf::Vector2f vel, pos;
    };

    struct QueuedEntity
    {
        sf::Vector2f pos;
        EntityID::ID id;

        QueuedEntity(sf::Vector2f pos_,
                     EntityID::ID id_) : pos(pos_), id(id_) {}

        QueuedEntity() = delete;
    };

    Entity(sf::Vector2f pos_, EntityID::ID ID_, bool isScripted_ = false, EntityScript *script_ = nullptr);
    Entity(sf::Vector2f pos_, EntityID::ID ID_ = EntityID::PIECE, sf::IntRect bounds = {});

    virtual ~Entity() override;

    // Static Methods
    static std::queue<QueuedEntity> &getQueue() { return entityQueue; }
    static const sf::IntRect         getBounds(EntityID::ID ID);
    static const sf::Vector2f        makeCenteredTL(sf::Vector2f pos, EntityID::ID);
    static const sf::Vector2f        getEVel(EntityID::ID ID_);
    static const EntityTarget        makePlayerTargetedVec( sf::Vector2f pos, EntityID::ID ID, uint8_t scale = 1, bool playerVelType = false);
    virtual void                     tick(double deltatime);
    virtual bool                     collide(Entity *other);
    virtual bool                     collide(sf::FloatRect otherBound);
    void                             setPos(sf::Vector2f newPos);
    void                             setVel(sf::Vector2f newPos);
    sf::Vector2f                     getPos() const { return pos; }
    sf::Vector2f                     getVel() const { return vel; }
    EntityID::ID                     getID() const { return ID; }
    const uint16_t                   getXP() const { return DATA_TABLE[ID].XP; }
    bool                             isOnScreen();
    virtual void                     draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
    struct EntityData
    {
        EntityData(Vec2<double> velocityFactor,
            uint16_t     xp) : VELOCITY_FACTOR(velocityFactor), XP(xp) {}

        EntityData() : VELOCITY_FACTOR(0.0, 0.0),
            XP(0) {}

        struct PlayerRef
        {
            static sf::Vector2f* pos;
            static sf::Vector2f* vel;
            static Entity* entity;
        };

        static constexpr Vec2<double> BASE_VELOCITY
        {
            100.f,
            100.f
        };
        static const PlayerRef PLAYER_REF;
        Vec2<double>           VELOCITY_FACTOR;
        uint16_t               XP;
    };

    sf::Vector2f              makeMiniDrawPos() const;
    static const EntityTarget makeTargetedVec(sf::Vector2f pos, EntityID::ID ID, Entity* other, uint8_t scale, bool velType);
    virtual void              applyFriction(double deltatime, double time);
    virtual void              wrap();

    // Static data members
    static const EntityData               DATA_TABLE[EntityID::LENGTH];
    static       sf::Sprite *             miniSprite;
    static       std::queue<QueuedEntity> entityQueue;

    sf::Vector2f       pos, vel;
    EntityScript *     script;
    VisualComponent* visual;
    bool               isScripted;
    const EntityID::ID ID;
};
