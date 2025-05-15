#pragma once
#include <concepts>
#include <vector>
#include "Action.h"
#include "DeathAnimation.h"
#include "EntityManager/EntityHolder.h"

#include "../Entity/Entity.hpp"

using ScoreType = uint64_t;


class EntityManager : public sf::Drawable
{
    class LanderTargetTable;

public:
    enum class PlayerState
    {
        ALIVE,
        RESPAWNING,
        DEAD
    };

    EntityManager(bool scripted_ = false);
    virtual ~EntityManager() override = default;


    template<typename ... Args>
    static void         spawn(bool particalize, EntityID::ID ID, sf::Vector2f pos, Args&&... args);
    static void         particleize(bool spawn, sf::Vector2f pos, EntityID::ID ID, sf::Vector2<int8_t> collision, Entity *entity);
    virtual void        draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    static PlayerState  tick(double deltatime, Action &actions);
    static uint8_t      astronautCount();
    static void         killAstronauts();
    static ScoreType &  getScore();
    static void         waveReset();
    static void         deathReset();
    static bool         waveComplete();
    static sf::Vector2f getPlayerPos();

private:
    // @todo If time permits, play with optimization, potentially using a spatial tree.
    template<typename T>
    static bool collisionWrapper(uint16_t projectile, EntityHolder<T> &entities);
    template<typename T>
    static bool validCollision(uint16_t entityIndex, Entity * entity, EntityHolder<T> &entities);
    static void enemyCollisionBranch(Enemy *enemy, uint16_t enemyIndex);
    static void astroCollisionBranch(Astronaut *astro, uint16_t astroIndex);
    static void clearQueue();
    static void spawn_typeWrapper(Entity *entity);
    static void tickLander(double deltatime, uint16_t index);
    static void tickPlayer(double deltatime, Action &actions);
    static void tickEntities(double deltatime);
    static void adjViewport(sf::View *view, double deltatime);
    static void killArea(sf::FloatRect viewport);
    static void hyperspace();


    static EntityHolder<Projectile>         projectiles;
    static EntityHolder<Entity>             enemies;
    static EntityHolder<Entity>             astronauts;
    static EntityHolder<Particle>           particles; // Always scripted
    static Player *                         player;
    static DeathAnimation *                 deathAnim;
    static UserInterface::EntityManagerData uiPassthrough;
    static constexpr ScoreType              rewardReq = 10000;
    static uint16_t                         lastReward;
    static LanderTargetTable                landerTargetTable;
    static uint16_t                         baiterCounter;
    static bool                             scripted;
    static PlayerState                      playerState;
    static const bool &                     isInvasion, &spawningComplete;
};


// ################################################
// ################################################
// ################################################
// ################# TEMPLATES ####################
// ################################################
// ################################################
// ################################################


template<typename T>
bool EntityManager::collisionWrapper(uint16_t         projectile,
                                     EntityHolder<T> &entities)
{
    uint16_t i    = 0;
    bool     died = false;

    while (i < entities.entities.size() && died != true)
    {
        //std::cout << entity_ << ' ' << projectiles.entities.at(projectile) << '\n';
        T *other = entities.entities.at(i);

        if (validCollision(projectile, other, entities))
        {
            if (dynamic_cast<Enemy*>(other))
                enemyCollisionBranch(dynamic_cast<Enemy*>(other), i);
            else if (dynamic_cast<Astronaut*>(other))
                astroCollisionBranch(dynamic_cast<Astronaut*>(other), i);

            // @todo Find collision point for particalization; Projectile method?
            particleize(
                    false,
                    other->getPos(),
                    other->getID(),
                    {
                        static_cast<int8_t>(projectiles.entities.at(projectile)->getPos().x - other->getPos().x),
                        static_cast<int8_t>(projectiles.entities.at(projectile)->getPos().y - other->getPos().y)
                    },
                    nullptr);

            projectiles.kill(projectile);
            entities.kill(i);

            died = true;
        }

        ++i;
    }

    return died;
}

template<typename T>
bool EntityManager::validCollision(uint16_t         entityIndex,
                                   Entity *         entity,
                                   EntityHolder<T> &entities)
{
    return (entity != nullptr && // If the entity is not dead
        !((projectiles.entities.at(entityIndex)->getID() == EntityID::LASER &&
                entity->getID() == EntityID::PLAYER)
            || // If its not a laser hitting a player
            (projectiles.entities.at(entityIndex)->getID() != EntityID::LASER &&
                entity->getID() != EntityID::PLAYER))
        && // or a non laser hitting a non player
        projectiles.entities.at(entityIndex)->collide(entity));
}


// @TODO Add bool for preventing particalization
template<typename ... Args>
void EntityManager::spawn(bool         particalize,
                          EntityID::ID ID,
                          sf::Vector2f pos,
                          Args && ...  args)
{
    Entity *entity;
    //std::cout << "SPAWNING..\n.";
    switch (ID)
    {
    case EntityID::PLAYER:
        //delete player;
        //std::cout << "SPAWNING PLAYER...";
        playerState = PlayerState::ALIVE;
        entity = new Player(pos, args ...);
        break;

    case EntityID::ASTRONAUT:
        entity = new Astronaut(pos, args ...);
        break;

    case EntityID::BULLET:
        projectiles.push<Bullet>(new Bullet(pos, args ...));
        return;

    case EntityID::LASER:
        projectiles.push<Laser>(new Laser(pos, args ...));
        return;

    case EntityID::BOMB:
        projectiles.push<Bomb>(new Bomb(pos, args ...));
        return;

    case EntityID::LANDER:
        entity = new Lander(pos, args ...);
        break;

    case EntityID::MUTANT:
        entity = new Mutant(pos, args ...);
        break;

    case EntityID::BAITER:
        ++baiterCounter;
        if (player)
            pos += player->getPos();

        if (pos.x > COMN::worldSize)
            pos.x -= COMN::worldSize;
        else if (pos.x < 0)
            pos.x += COMN::worldSize;

        if (pos.y > COMN::playHeight::max)
            pos.y -= COMN::playHeight::max - COMN::playHeight::min;
        else if (pos.y < COMN::playHeight::min)
            pos.y += COMN::playHeight::max - COMN::playHeight::min;

        entity = new Baiter(pos, args ...);
        break;

    case EntityID::BOMBER:
        entity = new Bomber(pos, args ...);
        break;

    case EntityID::POD:
        entity = new Pod(pos, args ...);
        break;

    case EntityID::SWARMER:
        entity = new Swarmer(pos, args ...);
        break;

    default:
        throw std::runtime_error(
                "Invalid SpawnType : EntityManager::spawn(sw)");
    }

    // @todo complete
    if (particalize)
        particleize(true, pos, ID, Particle::defCent(), entity);
    else
        spawn_typeWrapper(entity);
}
