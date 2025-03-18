#include "../EntityManager.h"

bool EntityManager::scripted = true;


EntityManager::EntityHolder<Projectile> EntityManager::projectiles;
EntityManager::EntityHolder<Enemy>      EntityManager::enemies;
EntityManager::EntityHolder<Astronaut>  EntityManager::astronauts;
EntityManager::EntityHolder<Entity>     EntityManager::particles; // Always scripted
Player* EntityManager::player = nullptr;

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

bool EntityManager::tick(Action& actions, double deltatime)
{
    bool playerDeath = false;
    uint16_t enemyIndex = 0;

    player->setActions(actions);

    player->tick(deltatime);

    // Tick enemies
    for (auto& enemy : enemies.entities)
    {
        std::cout << enemy << ", X: " << enemy->getPos().x << ", Y: " << enemy->getPos().y << '\n';
        if (enemy != nullptr)
        {
            if (dynamic_cast<Lander*>(enemy))
                ((Lander*)enemy)->tick(deltatime);


            enemy->tick(deltatime);
        }
    }

    // Tick astronauts
    for (auto& astronaut : astronauts.entities)
        if (astronaut != nullptr)
            astronaut->tick(deltatime);

    // Handles all entity collisions with projectiles
    for (uint16_t i = 0; i < projectiles.entities.size(); i++)
    {
	    if (projectiles.entities.at(i) != nullptr)
	    {
            if (!playerDeath)
                playerDeath = player->collide(projectiles.entities.at(i));


            collisionWrapper<Enemy>(i, enemies);
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

void EntityManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // @todo correct animation calling

    std::cout << "DRAW!!!\n";
    // Draw all astronauuts
    for (auto& astronaut : astronauts.entities)
        if (astronaut != nullptr)
	        target.draw(*astronaut, states);

    // Draw all enemies
    for (auto& enemy : enemies.entities)
    {
        std::cout << "ENTITY: ";
        if (enemy != nullptr)
        {
            std::cout << "YES";
            target.draw(*enemy, states);
        }

        std::cout << '\n';
    }

    // Draw all projectiles
    for (auto& projectile : projectiles.entities)
        if (projectile != nullptr)
	        target.draw(*projectile, states);

    // Draw all particles
    for (auto& particle : particles.entities)
        if (particle != nullptr)
            target.draw(*particle, states);

    // Draw the player
    target.draw(*player, states);
}

// Use for spawning entity or for death
void EntityManager::particleize(bool spawn, sf::Vector2f pos, EntityID::EntityID ID)
{
}

void EntityManager::killArea(sf::FloatRect viewport)
{
    for (uint16_t i = 0; i < enemies.entities.size(); i++)
        if (enemies.entities.at(i)->getPos().x > viewport.left && enemies.entities.at(i)->getPos().x < viewport.left + viewport.width)
            enemies.kill(i);
}

void EntityManager::hyperspace(sf::FloatRect viewport)
{
    player->setPos({ (float)(std::rand() % (int)viewport.left), (float)(std::rand() % (int)viewport.top)});
}

void EntityManager::spawn(SpawnType type, sf::Vector2f pos, EntityID::EntityID ID)
{
    std::cout << "SPAWN!\n";

    switch (type)
    {
    case SpawnType::PROJECTILE:
        switch (ID)
        {
		case EntityID::BULLET:
            projectiles.spawn<Bullet>(pos, ID);
            break;

		case EntityID::LASER:
            projectiles.spawn<Laser>(pos, ID);
            break;

        case EntityID::BOMB:
            projectiles.spawn<Bomb>(pos, ID);
            break;

		default:
			throw std::runtime_error("Invalid Type : EntityManager::spawn;PROJ(sw)");

        }
        break;
    case SpawnType::ENEMY:
        switch (ID)
        {
		case EntityID::LANDER:
            enemies.spawn<Lander>(pos, ID);
            break;

		case EntityID::MUTANT:
            enemies.spawn<Mutant>(pos, ID);
            break;

		case EntityID::BAITER:
            enemies.spawn<Baiter>(pos, ID);
            break;

		case EntityID::BOMBER:
            enemies.spawn<Bomber>(pos, ID);
			break;

		case EntityID::POD:
            enemies.spawn<Pod>(pos, ID);
			break;

		case EntityID::SWARMER:
            enemies.spawn<Swarmer>(pos, ID);
			break;

        default:
            throw std::runtime_error("Invalid Type : EntityManager::spawn;ENEM(sw)");

        }
        break;
    case SpawnType::ASTRONAUT:
        astronauts.spawn<Astronaut>(pos, ID);
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
        Entity::QueuedEntity& e = Entity::getQueue().front();

        switch (e.id)
        {
        case EntityID::BULLET:
            projectiles.entities.at(projectiles.spawn<Bullet>(e.pos, e.id))->setVel({
                static_cast<float>(cos(e.rot)),
                static_cast<float>(cos(e.rot))
                });
            break;

        case EntityID::LASER:
            projectiles.entities.at(projectiles.spawn<Laser>(e.pos, e.id))->setVel({
                static_cast<float>(cos(e.rot)),
                static_cast<float>(cos(e.rot))
                });
            break;

        case EntityID::BOMB:
            projectiles.entities.at(projectiles.spawn<Bomb>(e.pos, e.id))->setVel({
                static_cast<float>(cos(e.rot)),
                static_cast<float>(cos(e.rot))
                });
            break;

        default:
            throw std::runtime_error("Don't do that. : EntityManager::clearQueue()");
        }
    }
}
