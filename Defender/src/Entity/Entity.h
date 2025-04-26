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
           EntityScript *script_ = nullptr) : pos(pos_), isScripted(isScripted_),
                                             script(script_),
                                             animation(DATA_TABLE[ID_].SPRITE_DATA),
                                             ID(ID_)
    {

        // Not setting caused bug of player instantly losing a life
        animation.setPosition(pos);


        //std::cout << pos.x << ' ' << pos.y << '\n';

        //std::cout << "BOUNDS: " << DATA_TABLE[ID_].SPRITE_DATA.bounds.getPosition().x << ", " << DATA_TABLE[ID_].SPRITE_DATA.bounds.getPosition().y << ", " << DATA_TABLE[ID_].SPRITE_DATA.bounds.getSize().x << ", " << DATA_TABLE[ID_].SPRITE_DATA.bounds.getSize().y << '\n';

        //vel = { (float)EntityData::BASE_VELOCITY.x, (float)EntityData::BASE_VELOCITY.y };
        //vel.x *= (float)DATA_TABLE[ID_].VELOCITY_FACTOR.x;
    	//vel.y *= (float)DATA_TABLE[ID_].VELOCITY_FACTOR.y;
    }

    Entity(sf::Vector2f pos_, sf::IntRect bounds = {}) : pos(pos_),
                                                    isScripted(false),
                                                    script(nullptr),
                                                    animation(SpriteData{bounds, 1}),
                                                    ID(EntityID::PARTICLE)
    {
        //std::cout << "construct bounds: " << bounds.left << ", " << bounds.top << ", " << bounds.width << ", " << bounds.height << '\n';
    }

    static std::queue<QueuedEntity> &getQueue() { return entityQueue; }
    virtual void                     tick(double deltatime);
    virtual bool                     collide(Entity* other);
    virtual void wrap();
    static const sf::IntRect &       getBounds(EntityID::ID ID);
    void                             setPos(sf::Vector2f newPos);
    void                             setVel(sf::Vector2f newPos);
    sf::Vector2f                     getPos() { return pos; }
    sf::Vector2f                     getVel() { return vel; }
    EntityID::ID                     getID() { return ID; }
    const uint16_t                   getXP() { return DATA_TABLE[ID].XP; }
    static const sf::Vector2f makePlayerTargetedVec(sf::Vector2f pos, EntityID::ID ID, uint8_t scale);
    static const sf::Vector2f makeCenteredTL(sf::Vector2f pos, EntityID::ID);
    static const sf::Vector2f getEVel(EntityID::ID ID_)
    {
        sf::Vector2f ret = {
            (float)EntityData::BASE_VELOCITY.x,
            (float)EntityData::BASE_VELOCITY.y
        };


        ret.x *= (float)DATA_TABLE[ID_].VELOCITY_FACTOR.x;
        ret.y *= (float)DATA_TABLE[ID_].VELOCITY_FACTOR.y;

        return ret;
    }

protected:
    virtual void
        draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        sf::Vector2f bound = {
            DisplayManager::getView().getCenter().x - DisplayManager::getView().getSize().x / 2 - COMN::worldSize,
            DisplayManager::getView().getCenter().x + DisplayManager::getView().getSize().x / 2 - COMN::worldSize
        };

        float specialX = 0;
        if (pos.x < 0 + bound.y)
			specialX = pos.x + COMN::worldSize;
		else if (pos.x + DATA_TABLE[ID].SPRITE_DATA.bounds.width > COMN::worldSize + bound.x)
			specialX = pos.x - COMN::worldSize;

        target.draw(animation, states);
		if (specialX != 0)
		{
			animation.setPosition({ specialX, pos.y });
			target.draw(animation, states);
			// animation.setPosition(pos); // Should not be needed; is reset by tick
		}
    }
    bool isOnScreen();
    struct EntityData
    {
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
