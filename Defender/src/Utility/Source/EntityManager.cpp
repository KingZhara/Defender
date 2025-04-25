#include "../EntityManager.h"
#include "../common.h"

bool EntityManager::scripted = true;


EntityManager::EntityHolder<Projectile> EntityManager::projectiles;
EntityManager::EntityHolder<Enemy> EntityManager::enemies;
EntityManager::EntityHolder<Astronaut> EntityManager::astronauts;
EntityManager::EntityHolder<Particle> EntityManager::particles; // Always scripted
Player *EntityManager::player = nullptr;

// @todo Make score update
ScoreType EntityManager::score;

EntityManager::EntityManager(bool scripted_)
{
    scripted = scripted_;

    projectiles.reset();
    enemies.reset();
    astronauts.reset();
    particles.reset();

    delete player;
    player = nullptr;
}

void EntityManager::adjViewport(sf::View *view, double deltatime)
{
    // @todo Add starting movement freedom... Refactor to be better as well

    if (!player)
        return;

    static constexpr double playfieldFactor = 1. / 4.; // Side cutoff
	static constexpr double epsilon = 1; // Epsilon for the cutoff
	const  double centeredPlayer = player->getPos().x + Entity::getBounds(EntityID::PLAYER).width / 2.; // The players position centered to its hitbox
	//const  double left = view->getCenter().x - playfieldWidth / 2.; // Left side of playfield

    // Handling is based on the side
    if (!player->getDir())
    {
        view->setCenter((float)std::round(view->getCenter().x + (COMN::baseSpeed.x + player->getVel().x) * deltatime), view->getCenter().y);

        if (centeredPlayer - (view->getCenter().x - view->getSize().x * playfieldFactor) < epsilon)
            view->setCenter((float)std::round(centeredPlayer + view->getSize().x * playfieldFactor), view->getCenter().y);
    }
    else
    {
        view->setCenter((float)std::round(view->getCenter().x - (COMN::baseSpeed.x  - player->getVel().x) * deltatime), view->getCenter().y);

        if ((view->getCenter().x + view->getSize().x * playfieldFactor) - centeredPlayer < epsilon)
            view->setCenter((float)std::round(centeredPlayer - view->getSize().x * playfieldFactor), view->getCenter().y);
        
    }



	/*#################### VV The Fun Way VV ####################*/

    /*
    static double playfieldFactor    = 2. / 4.;
    static double normalizationWidth = 50;
    const  double playfieldWidth     = view->getSize().x - (view->getSize().x * playfieldFactor)          ; // Playfield width
    const  double centeredPlayer     = player->getPos().x + Entity::getBounds(EntityID::PLAYER).width / 2.; // The players position centered to its hitbox
    double        viewOffset         = player->getVel().x                                                 ; // View offset
    double        xOffset                                                                                 ; // Player position - playfield percentage
    bool   atEdge             = false;

    if (player->getVel().x > 0) // If moving right
    {
        // Player relative to the left playfield cutoff (center - field width / 2)
        xOffset = centeredPlayer - (view->getCenter().x - playfieldWidth / 2);

        // Set if it has reached its side
        if (xOffset <= 4.5) // 4.5 is a magic number for stabilization. it probably makes sense if you follow the viewport and player tick pipelines more closely, im not gonna.
            atEdge = true;

        // Apply a normalization width to prevent reaching the asymptote & a minimum value
        xOffset = std::max(xOffset + normalizationWidth, -1.);
    }
    else if (player->getVel().x < 0) // If moving left
    {
        // Player relative to the left playfield cutoff (center + field width / 2)
        xOffset = centeredPlayer - (view->getCenter().x + playfieldWidth / 2);

        // Set if it has reached its side
        if (xOffset >= -4.5)
            atEdge = true;

        // Apply a normalization width to prevent reaching the asymptote & a minimum value
        xOffset = std::min(xOffset - normalizationWidth, 1.);
    }
    else // If not moving
        return;

    // Offset MUST be positive
    if (xOffset < 0)
        xOffset = -xOffset;

    // Apply a scaling speed difference based off of the offset
    if (!atEdge)
    {
        viewOffset += player->getVel().x * xOffset / 100;


        // Finally move by dx, performing screen width wrapping as well as integer bound rounding.
        view->setCenter(
            static_cast<float>(
                static_cast<int>(std::round(view->getCenter().x + viewOffset)) %
                static_cast<int>(std::round(view->getSize().x * 9))),
            COMN::resolution.y / 2);
    }
    else
    {
        view->setCenter({
            (float)std::round(centeredPlayer + (player->getVel().x > 0 ? 1 : -1) * playfieldWidth / 2),
            COMN::resolution.y / 2
            });
    }
    */
}

bool EntityManager::tick(Action &actions, double deltatime, float center = 0)
{
    bool     playerDeath = false;
    uint16_t enemyIndex  = 0;

    //std::cout << "There are " << particles.entities.size() - particles.count << " particles\n";

    // Tick player first
    if (player)
    {
        player->setActions(actions);
        player->tick(deltatime);
    }

    // Tick enemies
    for (auto &enemy : enemies.entities)
    {
        if (enemy != nullptr)
        {
            if (dynamic_cast<Lander*>(enemy))
                dynamic_cast<Lander*>(enemy)->tick(deltatime);
            else if (dynamic_cast<Mutant*>(enemy))
                dynamic_cast<Mutant*>(enemy)->tick(deltatime);
            else if (dynamic_cast<Pod*>(enemy))
                dynamic_cast<Pod*>(enemy)->tick(deltatime);
            else if (dynamic_cast<Baiter*>(enemy))
                dynamic_cast<Baiter*>(enemy)->tick(deltatime);
            else if (dynamic_cast<Swarmer*>(enemy))
                dynamic_cast<Swarmer*>(enemy)->tick(deltatime);
            else if (dynamic_cast<Bomber*>(enemy))
                dynamic_cast<Bomber*>(enemy)->tick(deltatime);
            else
            {
                throw std::runtime_error(
                        "Failed to cast enemy! : EntityManager::Tick()");
            }
        }
    }

    // Tick astronauts
    for (auto &astronaut : astronauts.entities)
    {
        if (astronaut != nullptr)
            astronaut->tick(deltatime);
    }

    // Tick Particles
    for (uint16_t i = 0; i < particles.entities.size(); i++)
    {
        Particle*& particle = particles.entities[i];

        if (particle != nullptr)
        {
            particle->tick(deltatime);

            if (particle->isComplete())
            {
                spawn_typeWrapper(particle->getEntity());
                particles.kill(i);
            }
        }
    }

    // Tick projectiles
    for (uint16_t i = 0; i < projectiles.entities.size(); i++)
    {
        if (projectiles.entities.at(i) != nullptr)
        {
            projectiles.entities.at(i)->tick(deltatime);

            if (projectiles.entities.at(i)->getPos().y > COMN::resolution.y ||
                projectiles.entities.at(i)->getPos().y < 0 ||
                projectiles.entities.at(i)->getPos().x > center +
                COMN::resolution.x / 2 ||
                projectiles.entities.at(i)->getPos().x < center -
                COMN::resolution.x / 2)
                projectiles.kill(i);
        }
    }

    // Spawn all projectiles
    clearQueue();

    // Handles all entity collisions with projectiles
    for (uint16_t i = 0; i < projectiles.entities.size(); i++)
    {
        if (projectiles.entities.at(i) != nullptr)
        {
            //if (!playerDeath)
            //    playerDeath = player->collide(projectiles.entities.at(i));


            if (!playerDeath && !collisionWrapper<Enemy>(i, enemies))
                collisionWrapper<Astronaut>(i, astronauts);
        }
    }

    /*
    // Handles player collision with enemies
    if (!playerDeath)
    {
        while (!playerDeath && enemyIndex < enemies.entities.size())
        {
            if (enemies.entities.at(enemyIndex) != nullptr)
            {
                playerDeath = player->collide(enemies.entities.at(enemyIndex));
                if (playerDeath)
                    enemies.kill(enemyIndex);
            }
            ++enemyIndex;
        }
    }
    */
    return playerDeath;
}

void EntityManager::draw(sf::RenderTarget &target,
                         sf::RenderStates  states) const
{
    // @todo correct animation calling

    // Draw all astronauuts
    for (auto &astronaut : astronauts.entities)
    {
        if (astronaut != nullptr)
            target.draw(*astronaut, states);
    }

    // Draw all enemies
    for (auto &enemy : enemies.entities)
    {
        if (enemy != nullptr)
            target.draw(*enemy, states);
    }

    // Draw all projectiles
    for (auto &projectile : projectiles.entities)
    {
        if (projectile != nullptr)
            target.draw(*projectile, states);
    }

    // Draw all particles
    for (auto &particle : particles.entities)
    {
        if (particle != nullptr)
            target.draw(*particle, states);
    }

    // Draw the player
    if (player)
        target.draw(*player, states);
}

// Use for spawning entity or for death
// @todo test
// @todo make death behavior
void EntityManager::particleize(bool spawn, sf::Vector2f pos, EntityID::ID ID, sf::Vector2<int8_t> collision, Entity* entity)
{
    particles.spawn<Particle>(pos, ID, spawn, collision, entity);
}

void EntityManager::killArea(sf::FloatRect viewport)
{
    
    for (uint16_t i = 0; i < enemies.entities.size(); i++)
    {
        if (enemies.entities.at(i)->getPos().x > viewport.left &&
            enemies.entities.at(i)->getPos().x < viewport.left + viewport.width)
            enemies.kill(i);
    }
    
}

void EntityManager::hyperspace(sf::Vector2f size, float left)
{
    player->setPos({
        static_cast<float>(std::rand() % static_cast<int>(size.x + left)),
        static_cast<float>(std::rand() % static_cast<int>(size.y))
    });
}

ScoreType EntityManager::getScore()
{
    return score;
}

void EntityManager::clearQueue()
{
    while (!Entity::getQueue().empty())
    {
        Entity::QueuedEntity &e   = Entity::getQueue().front();
        double                rot = atan2(player->getPos().y - e.pos.y,
                                          player->getPos().x - e.pos.x);
        sf::Vector2f entityVel = Entity::getEVel(e.id);

        switch (e.id)
        {
        case EntityID::BULLET:
            projectiles.entities.at(projectiles.spawn<Bullet>(e.pos))->setVel({
                entityVel.x * static_cast<float>(cos(rot)) + player->getVel().x,
                entityVel.y * static_cast<float>(sin(rot)) + player->getVel().y
            });
            break;

        case EntityID::LASER:
            projectiles.entities.at(projectiles.spawn<Laser>(e.pos))->setVel({
                static_cast<float>(cos(rot)) * 3,
                static_cast<float>(sin(rot)) * 3
            });
            break;

        case EntityID::BOMB:
            projectiles.entities.at(projectiles.spawn<Bomb>(e.pos))->setVel({
                static_cast<float>(cos(rot)) * 3,
                static_cast<float>(sin(rot)) * 3
            });
            break;

        default:
            throw std::runtime_error( // Invalid entity was queued
                    "Don't do that. : EntityManager::clearQueue()");
        }

        Entity::getQueue().pop();
    }
}


void EntityManager::spawn_typeWrapper(Entity* entity)
{
    switch (entity->getID())
    {
    case EntityID::BULLET:
        projectiles.push<Bullet>(entity);
        break;

    case EntityID::LASER:
        projectiles.push<Laser>(entity);
        break;

    case EntityID::BOMB:
        projectiles.push<Bomb>(entity);
        break;

    case EntityID::LANDER:
        enemies.push<Lander>(entity);
        break;

    case EntityID::MUTANT:
        enemies.push<Mutant>(entity);
        break;

    case EntityID::BAITER:
        enemies.push<Baiter>(entity);
        break;

    case EntityID::BOMBER:
        enemies.push<Bomber>(entity);
        break;

    case EntityID::POD:
        enemies.push<Pod>(entity);
        break;

    case EntityID::SWARMER:
        enemies.push<Swarmer>(entity);
        break;

    case EntityID::ASTRONAUT:
        astronauts.push<Astronaut>(entity);
        break;
    case EntityID::PLAYER:
        delete player;
        player = (Player*)entity;
        break;

    default:
        throw std::runtime_error(
            "Invalid SpawnType : EntityManager::spawn(sw)");
    }
}