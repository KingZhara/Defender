#pragma once
#include <iostream>
#include <queue>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "../Utility/VisualComponent/Animation.h"
#include "../Utility/EntityID.h"
#include "../Utility/ShaderID.h"
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

    // @todo Complete shader retrieval through UI when possible
    Entity(sf::Vector2f  pos_,
           EntityID::ID  ID_,
           bool          isScripted_ = false,
           EntityScript *script_     = nullptr) : pos(pos_),
                                                  isScripted(isScripted_),
                                                  script(script_),
                                                  visual(new VisualComponent(ID_)),
                                                  ID(ID_)
    {
        visual->setPosition(pos);

        // Lazy initialization of the miniSprite
        if (!miniSprite)
            miniSprite = new sf::Sprite(*DisplayManager::getTexture(),
                                        sf::IntRect(0, 0, 2, 2));
    }

    Entity(sf::Vector2f pos_,
           EntityID::ID ID_    = EntityID::PIECE,
           sf::IntRect  bounds = {}) : pos(pos_),
                                       isScripted(false),
                                       script(nullptr),
                                       visual(new PieceVisualComponent(bounds, ID_)),
                                       ID(ID_) {}

    // Static Methods

    static std::queue<QueuedEntity> &getQueue() { return entityQueue; }
    static const sf::IntRect&        getBounds(EntityID::ID ID);
    static const sf::Vector2f        makeCenteredTL(sf::Vector2f pos, EntityID::ID);
    static const sf::Vector2f        getEVel(EntityID::ID ID_);
    static const EntityTarget        makePlayerTargetedVec(
        sf::Vector2f pos,
        EntityID::ID ID,
        uint8_t      scale = 1,
        bool         playerVelType = false);

    // 
    virtual void   tick(double deltatime);
    virtual bool   collide(Entity *other);
    virtual void   applyFriction(double deltatime, double time);
    virtual bool   collide(sf::FloatRect otherBound); // Used by Laser
    virtual void   wrap();
    void           setPos(sf::Vector2f newPos);
    void           setVel(sf::Vector2f newPos);
    sf::Vector2f   getPos() const { return pos; }
    sf::Vector2f   getVel() const { return vel; }
    EntityID::ID   getID() const { return ID; }
    const uint16_t getXP() const { return DATA_TABLE[ID].XP; }
    bool           isOnScreen();

    virtual void
        draw(sf::RenderTarget& target, sf::RenderStates states) const override;
protected:

    sf::Vector2f makeMiniDrawPos() const;
    static const EntityTarget        makeTargetedVec(sf::Vector2f pos, EntityID::ID ID, Entity* other,
        uint8_t scale, bool velType);

    struct EntityData
    {
        EntityData(SpriteData   spriteData,
            Vec2<double> velocityFactor,
            uint16_t     xp) : SPRITE_DATA(spriteData),
            VELOCITY_FACTOR(velocityFactor), XP(xp) {}

        EntityData() : SPRITE_DATA({ 0, 0, 0, 0 }, 0),
            VELOCITY_FACTOR(0.0, 0.0),
            XP(0) {}

        struct PlayerRef
        {
            static sf::Vector2f *pos;
            static sf::Vector2f *vel;
			static Entity* entity;
        };

        static constexpr Vec2<double> BASE_VELOCITY
        {
            100.f,
            100.f
        };
        static const PlayerRef PLAYER_REF;
        SpriteData             SPRITE_DATA;
        Vec2<double>           VELOCITY_FACTOR;
        uint16_t               XP;
    };


    // The data table used for generating a given sprite
    static const EntityData DATA_TABLE[
        EntityID::LENGTH]; // i REALLY tried to make this constexpr
    static sf::Sprite *             miniSprite;
    static std::queue<QueuedEntity> entityQueue;

    sf::Vector2f       pos, vel;
    bool               isScripted;
    EntityScript *     script;
    VisualComponent* visual;
    const EntityID::ID ID;
};
