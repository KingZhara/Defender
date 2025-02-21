#include "../EntityManager.h"

EntityManager::EntityManager(bool scripted_) : scripted(scripted_)
{
    player = new Player({ 50, 50 });
}

bool EntityManager::tick(Action& actions)
{
    bool playerDeath = false;

    player->setActions(actions);

    player->tick();

    // Tick enemies
    for (auto& enemy : enemies.entities)
        if (enemy != nullptr)
			enemy->tick();

    // Tick astronauts
    for (auto& astronaut : astronauts.entities)
        if (astronaut != nullptr)
            astronaut->tick();

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


    return playerDeath;
}

void EntityManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto& astronaut : astronauts.entities)
        target.draw(*astronaut, states);

	for (auto& enemy : enemies.entities)
        target.draw(*enemy, states);

    for (auto& projectile : projectiles.entities)
        target.draw(*projectile, states);

    target.draw(*player, states);
}

// Use for spawning entity or for death
void EntityManager::particleize(bool spawn, sf::Vector2f pos, EntityID::EntityID ID)
{
}