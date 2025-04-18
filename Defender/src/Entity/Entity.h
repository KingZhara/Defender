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
        EntityID::ID id;

		QueuedEntity(sf::Vector2f pos_, EntityID::ID id_) : pos(pos_), id(id_) {}
        QueuedEntity() = delete;
    };

    // @todo Complete shader retrieval through UI when possible
    Entity(sf::Vector2f  pos_,
           EntityID::ID  ID_,
           bool          isScripted_ = false,
           EntityScript *script_ = nullptr,
           sf::Shader   *shader = nullptr) : pos(pos_), isScripted(isScripted_),
                                             script(script_),
                                             animation(
                                                     DATA_TABLE[ID_].SPRITE_DATA.
                                                     frameCount,
                                                     DATA_TABLE[ID_].SPRITE_DATA.bounds,
                                                     ID_,
                                                     DATA_TABLE[ID_].SPRITE_DATA.
                                                     frameLength
                                                     /*SPRITE_TABLE[ID].shader*/,
                                                     shader),
                                             ID(ID_)
    {
        //std::cout << pos.x << ' ' << pos.y << '\n';

        std::cout << "BOUNDS: " << DATA_TABLE[ID_].SPRITE_DATA.bounds.getPosition().x << ", " << DATA_TABLE[ID_].SPRITE_DATA.bounds.getPosition().y << ", " << DATA_TABLE[ID_].SPRITE_DATA.bounds.getSize().x << ", " << DATA_TABLE[ID_].SPRITE_DATA.bounds.getSize().y << '\n';
    }

    Entity(sf::Vector2f pos_, sf::IntRect bounds = {}) : pos(pos_),
                                                    isScripted(false),
                                                    script(nullptr),
                                                    animation({1, bounds, EntityID::PARTICLE}),
                                                    ID(EntityID::PARTICLE) {}

    static std::queue<QueuedEntity> &getQueue() { return entityQueue; }
    virtual void                     tick(double deltatime);
    virtual bool                     collide(Entity* other);
    static const sf::IntRect &       getBounds(EntityID::ID ID);
    void                             setPos(sf::Vector2f newPos);
    void                             setVel(sf::Vector2f newPos);
    sf::Vector2f                     getPos() { return pos; }
    sf::Vector2f                     getVel() { return vel; }
    EntityID::ID                     getID() { return ID; }
    const uint16_t                   getXP() { return DATA_TABLE[ID].XP; }

protected:
    virtual void
        draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        target.draw(animation, states);
    }

    void setShader(sf::Shader* shader_)
    {
        animation.setShader(shader_);
    }
    struct EntityData
    {
        struct SpriteData
        {
            sf::IntRect        bounds;
            uint8_t            frameCount;
            ShaderID::ID shader = ShaderID::NONE;
            double             frameLength = 1. / 2.;

			SpriteData(sf::IntRect bounds_, uint8_t frameCount_,
				ShaderID::ID shader_ = ShaderID::NONE,
				double frameLength_ = 1. / 2.) :
				bounds(bounds_), frameCount(frameCount_), shader(shader_),
				frameLength(frameLength_)
			{
			}
        };

		struct PlayerRef
		{
			static sf::Vector2f* pos;
			static sf::Vector2f* vel;
		};

        static constexpr Vec2<double> BASE_VELOCITY
        {
            150.f,
            100.f
        };

        static const PlayerRef PLAYER_REF;
		SpriteData SPRITE_DATA;
        Vec2<double> VELOCITY_FACTOR;
        uint16_t XP;

		EntityData(SpriteData spriteData, Vec2<double> velocityFactor, uint16_t xp) :
			SPRITE_DATA(spriteData), VELOCITY_FACTOR(velocityFactor), XP(xp)
		{
		    
		}
        EntityData() :
            SPRITE_DATA({ 0,0,0,0 }, 0),
            VELOCITY_FACTOR(0.0, 0.0),
            XP(0)
        {}
    };


    // The data table used for generating a given sprite
    static const EntityData DATA_TABLE[EntityID::LENGTH]; // i REALLY tried to make this constexpr
    static std::queue<QueuedEntity> entityQueue;

    sf::Vector2f       pos, vel;
    bool               isScripted;
    EntityScript *     script;
    Animation          animation;
    const EntityID::ID ID;
};
