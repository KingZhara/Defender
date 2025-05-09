#include "../EntityManager.h"
#include "../common.h"
#include "../DeathAnimation.h"

bool EntityManager::scripted = true;


EntityManager::EntityHolder<Projectile> EntityManager::projectiles;
EntityManager::EntityHolder<Entity> EntityManager::enemies;
EntityManager::EntityHolder<Entity> EntityManager::astronauts;
EntityManager::EntityHolder<Particle> EntityManager::particles; // Always scripted
Player *EntityManager::player = nullptr;
EntityManager::LanderTargetTable EntityManager::landerTargetTable;
uint16_t EntityManager::baiterCounter = 0;
// @todo Make score update
//ScoreType EntityManager::score;
EntityManager::PlayerState EntityManager::playerState = PlayerState::DEAD;
DeathAnimation* EntityManager::deathAnim = nullptr;
UserInterface::EntityManagerData EntityManager::uiPassthrough;
uint16_t EntityManager::lastReward = 1;

EntityManager::EntityManager(bool scripted_)
{
    scripted = scripted_;

    projectiles.reset();
    enemies.reset();
    astronauts.reset();
    particles.reset();

	landerTargetTable.landerToAstronaut.clear();
	landerTargetTable.astronautToLander.clear();

    delete player;
    player = nullptr;

    uiPassthrough = UserInterface::EntityManagerData();
    baiterCounter = 0;
}

void EntityManager::adjViewport(sf::View *view, double deltatime)
{
    // @todo Add starting movement freedom... Refactor to be better as well

  
    //if (!player)
      //  return;

    static constexpr double playfieldFactor = 1. / 4.; // Side cutoff
    static int8_t viewOffset = 0;

    if (!player)
        return;

    const int8_t maxOffset = static_cast<int8_t>(COMN::resolution.x * playfieldFactor);
    const double centeredPlayer = (player->getPos().x + Entity::getBounds(EntityID::PLAYER).width / 2.);

    // Update offset
    if (player->getDir()) // Facing left
    {
        if (viewOffset > -maxOffset)
            --viewOffset;
    }
    else // Facing right
    {
        if (viewOffset < maxOffset)
            ++viewOffset;
    }

    // Apply offset to view
    float viewCenterX = static_cast<float>(centeredPlayer + viewOffset);
    view->setCenter(std::round(viewCenterX), std::round(view->getCenter().y));

	//view->setCenter(std::round(view->getCenter().x), view->getCenter().y);

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

EntityManager::PlayerState EntityManager::tick(double deltatime, Action& actions)
{
    // TMEPORARY DECLARATION
    //playerState = PlayerState::ALIVE;
	//std::cout << particles.entities.size() << " particles\n";
    //uint16_t enemyIndex  = 0;

    //std::cout << "There are " << particles.entities.size() - particles.count << " particles\n";

    // Tick player first
    if (playerState == PlayerState::ALIVE)
    {

        if (player)
            player->tick(deltatime);

        // Change playerState to be a player state -> make default to dead, handle in constructor... and spawn methods


        // if player is respawning perform animation, if dead, reset data (maybe? look at constructor and reset methods to determine if necessary...) return accordingly.
        // handle death animation interactions with user interface
        // Handle UI changes..
        // 
        tickEntities(deltatime);
        tickPlayer(deltatime, actions);
    }
    if (playerState != PlayerState::ALIVE)
    {
        if (!deathAnim)
        {
            deathAnim = new DeathAnimation(player->getPos());
            UserInterface::startDeathAnimation();
        }
        else if (!UserInterface::isDeathAnimCompleted())
            deathAnim->tick(deltatime);
        else
            deathReset();

		//std::cout << "DEATH ANIM: " << deathAnim << ", COMPL: " << UserInterface::isDeathAnimCompleted() << '\n';
    }


    //playerState = PlayerState::ALIVE;

    if (playerState == PlayerState::DEAD && deathAnim)
        return PlayerState::RESPAWNING;

    return playerState;
}


void EntityManager::tickEntities(double deltatime)
{
    bool     playerDeath = false;

    // Tick enemies
    for (uint16_t i = 0; i < enemies.entities.size(); i++)
    {
        if (enemies.entities.at(i) != nullptr)
        {
            if (dynamic_cast<Lander*>(enemies.entities.at(i)))
                tickLander(deltatime, i);
            else
                enemies.entities.at(i)->tick(deltatime);
            if (dynamic_cast<Particle*>(enemies.entities.at(i)))
            {
                Particle* particle = dynamic_cast<Particle*>(enemies.entities.at(i));
                if (particle->isComplete())
                    enemies.spawn(i, particle->getEntity()); // Spawn the entity that was particleized
            }
        }
    }

    // @todo Needs to detect death
    for (uint16_t i = 0; i < astronauts.entities.size(); i++)
    {
        Entity*& entity = astronauts.entities.at(i);

        if (entity != nullptr)
        {
            entity->tick(deltatime);
            if (dynamic_cast<Particle*>(entity))
            {
                Particle* particle = dynamic_cast<Particle*>(entity);
                if (particle->isComplete())
                    astronauts.spawn(i, particle->getEntity()); // Spawn the entity that was particleized
            }
        }
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
            Projectile& projectile = *projectiles.entities.at(i);
            sf::Vector2f pos = projectile.getPos();

            if (!projectile.isOnScreen())
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
            if (player && !playerDeath && projectiles.entities.at(i)->getID() != EntityID::LASER)
                playerDeath = player->collide(projectiles.entities.at(i));


            if (!playerDeath && !collisionWrapper<Entity>(i, enemies))
                collisionWrapper<Entity>(i, astronauts);
        }

        if (playerDeath)
            break;
    }

    if (player)
    {
        for (uint16_t i = 0; i < enemies.entities.size(); ++i)
        {
            if (playerDeath)
                break;

            if (enemies.entities.at(i))
            {
                playerDeath = player->collide(enemies.entities.at(i));
                if (playerDeath)
                {
                    if (enemies.entities.at(i)->getID() == EntityID::BAITER)
                        --baiterCounter;

                    uiPassthrough.score += enemies.entities[i]->getXP();
                    enemies.kill(i);
                }
            }
        }
    }

    if (playerDeath)
        playerState = PlayerState::RESPAWNING; // Updates in playerTick
}

void EntityManager::tickPlayer(double deltatime, Action& actions)
{
    //static Timer<double> hyperspaceCooldown;
    // DO NOT CALL IOF PLAYER IS DESD; THAT IS HANDLES WITH DEATH ANIMATION

    if (playerState == PlayerState::ALIVE)
    {
        // ##############################
        // ####  Handle  Keypresses  ####
        // ##############################

        // @todo check if a cooldown was needed, I am just assuming it is - Ricky
        // Handle the hyperspace cooldown
        //if (!hyperspaceCooldown.isComplete())
        //    hyperspaceCooldown.tick(deltatime);

        // Execute hyperspace if applicable
        if (actions.flags.hyperspace)// && hyperspaceCooldown.isComplete())
        {
            hyperspace();
            actions.flags.hyperspace = false;
            //hyperspaceCooldown.tick(0);
        }

        // Handle and update smart bombs accordingly
        if (actions.flags.smart_bomb && uiPassthrough.smartBombs > 0)
        {
            killArea(DisplayManager::getView().getViewport());
            --uiPassthrough.smartBombs;
        }

        // ##############################
        // #######  Handle Score  #######
        // ##############################

        if (uiPassthrough.score >= rewardReq * lastReward)
        {
            ++lastReward;

            if (uiPassthrough.smartBombs != 255)
                ++uiPassthrough.smartBombs;
            if (uiPassthrough.extraLives != 255)
                ++uiPassthrough.extraLives;
        }
    }

    // Should handle saving the high score if needed
    if (playerState == PlayerState::RESPAWNING)
    {
        if (uiPassthrough.extraLives >= 1)
            --uiPassthrough.extraLives;
		else // Game Over; Reset Player
            playerState = PlayerState::DEAD;

        //playerState = false;
    }
    else
        adjViewport(&DisplayManager::getView(), deltatime);
}

void EntityManager::tickLander(double deltatime, uint16_t index)
{
    Lander* entity = dynamic_cast<Lander*>(enemies.entities.at(index));
    Astronaut* newTarget = nullptr;
    uint16_t astroIndex = 0;
    int16_t minDx = -1;
    bool mutate = false;

    if (entity->shouldMutate() || isInvasion)
    {
        spawn(false, EntityID::MUTANT, entity->getPos());

        if (!isInvasion)
            astronauts.kill(landerTargetTable.landerToAstronaut[index]);
        else
            spawn(false, EntityID::MUTANT, entity->getPos());


        enemies.kill(index);

        landerTargetTable.astronautToLander.erase(landerTargetTable.landerToAstronaut[index]);
        landerTargetTable.landerToAstronaut.erase(index);
        return;

    }

    if (!entity->hasTarget())
    {
        for (uint16_t i = 0; i < astronauts.entities.size(); ++i)
        {
            Astronaut* astronaut = dynamic_cast<Astronaut*>(astronauts.entities.at(i));
            if (astronaut && !astronaut->targeted())
            {
				int16_t dx = std::abs(astronaut->getPos().x - entity->getPos().x);
				if (minDx == -1 || dx < minDx)
				{
					minDx = dx;
					newTarget = astronaut;
					astroIndex = i;
				}
            }

        }
    }

    if (newTarget)
    {
        entity->setTarget(newTarget);
		dynamic_cast<Astronaut*>(astronauts.entities.at(astroIndex))->setTargeted(true);
        landerTargetTable.landerToAstronaut[index] = astroIndex;
        landerTargetTable.astronautToLander[astroIndex] = index;
    }

    enemies.entities.at(index)->tick(deltatime);
}

void EntityManager::draw(sf::RenderTarget &target,
                         sf::RenderStates  states) const
{
    // @todo correct animation calling

    // Draw all astronauuts
    for (auto& astronaut : astronauts.entities)
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
    if (player)
    	uiPassthrough.minimapOffset = (COMN::worldSize / 2 - DisplayManager::getView().getCenter().x) / COMN::worldSize * UserInterface::UIBounds::MINIMAP_WIDTH;

    if (deathAnim)
        target.draw(*deathAnim, states);

    UserInterface::drawForeground(target, DisplayManager::getView(), uiPassthrough);
}

// Use for spawning entity or for death
// @todo test
// @todo make death behavior
void EntityManager::particleize(bool spawn, sf::Vector2f pos, EntityID::ID ID, sf::Vector2<int8_t> collision, Entity* entity)
{

    if (spawn)
    {
        if (ID == EntityID::ASTRONAUT)
            astronauts.spawn<Particle>(pos, ID, spawn, collision, entity);
        else if (ID >= EntityID::LANDER && ID <= EntityID::SWARMER)
            enemies.spawn<Particle>(pos, ID, spawn, collision, entity);
        else
            particles.spawn<Particle>(pos, ID, spawn, collision, entity);
    }
    else
        particles.spawn<Particle>(pos, ID, spawn, collision, entity);

}

void EntityManager::killArea(sf::FloatRect viewport)
{
    
    for (uint16_t i = 0; i < enemies.entities.size(); i++)
    {

        if (enemies.entities.at(i) &&
            enemies.entities.at(i)->getPos().x > viewport.left &&
            enemies.entities.at(i)->getPos().x < viewport.left + viewport.width)
            enemies.kill(i);
    }
    
}

void EntityManager::hyperspace()
{
    // add chance to go wrong
    // make random board position
    if (player)
    {
        player->setPos({
            static_cast<float>(std::rand() % COMN::worldSize),
            static_cast<float>(std::rand() % (int)COMN::uiHeight)
            });
    }
}

ScoreType& EntityManager::getScore()
{
    return uiPassthrough.score;
}

void EntityManager::waveReset()
{
	projectiles.reset();
    enemies.reset();
	particles.reset();
    baiterCounter = 0;
}

void EntityManager::deathReset()
{
    delete deathAnim;
    deathAnim = nullptr;

    // Figure out wht death isnt working,this is likely an issue with how DEAD is being set, and stage state reading it.

    if (playerState != PlayerState::DEAD)
    	playerState = PlayerState::ALIVE;

	projectiles.reset();
	enemies.zero();
	particles.zero();
}

bool EntityManager::waveComplete()
{
	//std::cout << "Wave complete: " << enemies.getLiveCount() - baiterCounter << ", SPCOMPL: " << (spawningComplete ? "true" : "false") << '\n';
    return (enemies.getLiveCount() - baiterCounter == 0 && spawningComplete);
}

void EntityManager::clearQueue()
{
    while (!Entity::getQueue().empty())
    {
        Entity::QueuedEntity &e   = Entity::getQueue().front();

        switch (e.id)
        {
        case EntityID::BULLET:
            projectiles.entities.at(projectiles.spawn<Bullet>(e.pos))->setVel({
                Entity::makePlayerTargetedVec(e.pos, e.id, 1).vel
            });
            break;

        case EntityID::LASER:
            projectiles.entities.at(projectiles.spawn<Laser>(e.pos))->setVel({
                (player->getDir() ? -1 : 1) * Entity::getEVel(EntityID::LASER).x,
                0
            });
            break;

        case EntityID::BOMB:
            projectiles.entities.at(projectiles.spawn<Bomb>(e.pos))->setVel({
                Entity::makePlayerTargetedVec(e.pos, e.id, 0.1).vel
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
    std::cout << entity << '\n';

    if (entity == nullptr)
        return;

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
 		std::cout << "Player Spawned\n";
        delete player;
        player = (Player*)entity;
        player->setPos({ COMN::worldSize / 2, (COMN::resolution.y - COMN::uiHeight) / 2 });
        break;

    default:
        throw std::runtime_error(
            "Invalid SpawnType : EntityManager::spawn(sw)");
    }
}