#pragma once
#include <iostream>
#include <queue>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "../Utility/Animation.h"
#include "../Utility/EntityID.h"
#include "../Utility/ShaderID.h"
#include "../Utility/EntityScript/EntityScript.h"

// new EntityScript(EntityScript::ScriptType::DONE) - BLANK SCRIPT

class Entity : public sf::Drawable
{
public:
    struct QueuedEntity
    {
        sf::Vector2f pos;
        double       rot;
        EntityID::ID id;
    };

    // @todo Complete shader retrieval through UI when possible
    Entity(sf::Vector2f  pos_,
           EntityID::ID  ID_,
           bool          isScripted_ = false,
           EntityScript *script_ = nullptr,
           sf::Shader *  shader = nullptr) : pos(pos_), isScripted(isScripted_),
                                             script(script_),
                                             animation(
                                                     SPRITE_TABLE[ID_].
                                                     frameCount,
                                                     SPRITE_TABLE[ID_].bounds,
                                                     SPRITE_TABLE[ID_].
                                                     frameLength
                                                     /*SPRITE_TABLE[ID].shader*/,
                                                     shader),
                                             ID(ID_) {}

    Entity(sf::Vector2f pos_, sf::IntRect bounds) : pos(pos_),
                                                    isScripted(false),
                                                    script(nullptr),
                                                    animation({1, bounds}),
                                                    ID(EntityID::PARTICLE) {}

    static std::queue<QueuedEntity> &getQueue() { return entityQueue; }
    virtual void                     tick(double deltatime);
    bool                             collide(Entity *other);
    static const sf::IntRect &       getBounds(EntityID::ID ID);
    void                             setPos(sf::Vector2f newPos);
    void                             setVel(sf::Vector2f newPos);
    sf::Vector2f                     getPos() { return pos; }
    EntityID::ID                     getID() { return ID; }
    const uint16_t                   getXP() { return XP_TABLE[ID]; }

protected:
    virtual void
        draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        target.draw(animation, states);
    }

    struct SpriteData
    {
        sf::IntRect        bounds;
        uint8_t            frameCount;
        ShaderID::ShaderID shader      = ShaderID::NONE;
        double             frameLength = 1. / 2.;
    };

    // The data table used for generating a given sprite
    static const SpriteData         SPRITE_TABLE[EntityID::LENGTH];
    static const uint16_t           XP_TABLE[EntityID::LENGTH];
    static sf::Vector2f *           playerPos;
    static std::queue<QueuedEntity> entityQueue;

    sf::Vector2f       pos, vel;
    bool               isScripted;
    EntityScript *     script;
    Animation          animation;
    const EntityID::ID ID;
};
