#include "../EntityManager.h"
#include "../common.h"

bool EntityManager::scripted = true;


EntityManager::EntityHolder<Projectile> EntityManager::projectiles;
EntityManager::EntityHolder<Enemy> EntityManager::enemies;
EntityManager::EntityHolder<Astronaut> EntityManager::astronauts;
EntityManager::EntityHolder<Entity> EntityManager::particles; // Always scripted
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

void EntityManager::adjViewport(sf::View *view)
{
    // The view follows the player, moving at a varying speed and keeping the player with a minimum of 1/4 of the screen on either side.
    // the views movement speed seems proportional to the distance of the player from its position on the sorrect side, and the players movement itself; not moving? no change.

    // The goal here is to generate a motion factor and then apply it

    // get player direction
    // get player position
    // get player velocity
    // get offset from opposing side
    // view speed should be player speed + player speed * playfield width/(playfield width - offset)

    // if the player is outside of the view range, move the view accordingly
    // otherwise, move the view at a speed proportional to the player's speed as described above.

    // view offset
    double viewOffset = 0;

    // player position - playfield percentage
    uint16_t offsetFactor = 0;

    // playfield width
    uint16_t playfieldWidth = view->getSize().x / 2;


    if (player->getPos().x < view->getCenter().x - view->getSize().x / 4)
        viewOffset = player->getPos().x - (view->getCenter().x - view->getSize()
                                                                      .x / 4);
    else if (player->getPos().x > view->getCenter().x + view->getSize().x / 4)
        viewOffset = player->getPos().x - (view->getCenter().x + view->getSize()
                                                                      .x / 4);
    else // Within range
    {
        // Calculate offset factor
        if (player->getVel().x > 0)
            offsetFactor = player->getPos().x - view->getCenter().x
                                              - view->getSize().x / 4;
        else if (player->getVel().x < 0)
            offsetFactor = player->getPos().x - view->getCenter().x
                                              + view->getSize().x / 4;


        viewOffset = player->getVel().x + (playfieldWidth - offsetFactor == 0
                                               ? 0
                                               : player->getVel().x *
                                                     playfieldWidth /
                                                    (playfieldWidth -
                                                     offsetFactor));
    }

    std::cout << "ADJUSTING... " << viewOffset << '\n';

    view->move(viewOffset, 0);
}

bool EntityManager::tick(Action &actions, double deltatime, float center = 0)
{
    bool     playerDeath = false;
    uint16_t enemyIndex  = 0;

    // Tick player first
    player->setActions(actions);
    player->tick(deltatime);

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
            if (!playerDeath)
                playerDeath = player->collide(projectiles.entities.at(i));


            if (!playerDeath && !collisionWrapper<Enemy>(i, enemies))
                collisionWrapper<Astronaut>(i, astronauts);
        }
    }

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
    target.draw(*player, states);
}

// Use for spawning entity or for death
// @todo test
// @todo make death behavior
void EntityManager::particleize(bool spawn, sf::Vector2f pos, EntityID::ID ID)
{
    sf::IntRect bounds = Entity::getBounds(ID);
    int8_t      row    = 0, col = 0;

    do
    {
        do
        {
            particles.entities.at(particles.spawn<Entity>(
                    {
                        pos.x + col,
                        pos.y + row
                    },
                    sf::IntRect{
                        bounds.left + col,
                        bounds.top + row,
                        2 % (bounds.width - col + 1),
                        2 % (bounds.height - row + 1)
                    }))->setVel(
            {
                static_cast<float>(col - bounds.width / 2.),
                static_cast<float>(row - bounds.height / 2.),
            });


            row += 2;
        }
        while (col <= bounds.height);

        col = 0;
        row += 2;
    }
    while (row <= bounds.width);
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

void EntityManager::spawn(SpawnType type, sf::Vector2f pos, EntityID::ID ID)
{
    switch (type)
    {
    case SpawnType::PROJECTILE:
        switch (ID)
        {
        case EntityID::BULLET:
            projectiles.spawn<Bullet>(pos);
            break;

        case EntityID::LASER:
            projectiles.spawn<Laser>(pos);
            break;

        case EntityID::BOMB:
            projectiles.spawn<Bomb>(pos);
            break;

        default:
            throw std::runtime_error(
                    "Invalid Type : EntityManager::spawn;PROJ(sw)");
        }
        break;
    case SpawnType::ENEMY:
        switch (ID)
        {
        case EntityID::LANDER:
            enemies.spawn<Lander>(pos);
            break;

        case EntityID::MUTANT:
            enemies.spawn<Mutant>(pos);
            break;

        case EntityID::BAITER:
            enemies.spawn<Baiter>(pos);
            break;

        case EntityID::BOMBER:
            enemies.spawn<Bomber>(pos);
            break;

        case EntityID::POD:
            enemies.spawn<Pod>(pos);
            break;

        case EntityID::SWARMER:
            enemies.spawn<Swarmer>(pos);
            break;

        default:
            throw std::runtime_error(
                    "Invalid Type : EntityManager::spawn;ENEM(sw)");
        }
        break;
    case SpawnType::ASTRONAUT:
        astronauts.spawn<Astronaut>(pos);
        break;
    case SpawnType::PLAYER:
        player = new Player(pos);
        break;
    }
}

void EntityManager::clearQueue()
{
    while (!Entity::getQueue().empty())
    {
        Entity::QueuedEntity &e   = Entity::getQueue().front();
        double                rot = atan2(player->getPos().y - e.pos.y,
                                          player->getPos().x - e.pos.x);

        switch (e.id)
        {
        case EntityID::BULLET:
            projectiles.entities.at(projectiles.spawn<Bullet>(e.pos))->setVel({
                static_cast<float>(cos(rot)) * 2 + player->getVel().x,
                static_cast<float>(sin(rot)) * 2 + player->getVel().y
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
