#include "../Mutant.h"

void Mutant::tick(double deltatime)
{
    static Timer<double>      attackTimer{2};
    static bool               type  = true;

    // Bob up or down on even intervals
    if (bobStage % 2 == 0)
    {
        if (bobStage <= 3)
            pos.y += 1;
        else
            pos.y -= 1;
    }

    // Update
    bobStage = (bobStage + 1) % 8;
    
    // Attack
    if (attackTimer.tick(deltatime))
        entityQueue.emplace(QueuedEntity( makeCenteredTL(pos, EntityID::MUTANT), EntityID::BULLET ));
    

    // Movement
    // If above the player
    
    if (abs(pos.x - EntityData::PLAYER_REF.pos->x) <
        4) // 2 = square radius on the x-axis around the player
    {
        if (pos.x - EntityData::PLAYER_REF.pos->x < -2)
            vel.x = COMN::baseSpeed.x;
        else if (pos.x - EntityData::PLAYER_REF.pos->x > 2)
            vel.x = -COMN::baseSpeed.x;

        if (pos.y - EntityData::PLAYER_REF.pos->y < 0)
            vel.y = COMN::baseSpeed.y;
        else
            vel.y = -COMN::baseSpeed.y;
    }
    else
    {
        if (pos.x - EntityData::PLAYER_REF.pos->x < 0)
            vel.x = COMN::baseSpeed.x;
        else
            vel.x = -COMN::baseSpeed.x;

        // If we are out of the ideal range (screen height / 8 +- 5)
        if (pos.y - EntityData::PLAYER_REF.pos->y >  COMN::resolution.y / 8 + 5 || 
            pos.y - EntityData::PLAYER_REF.pos->y < -COMN::resolution.y / 8 - 5)
        {
            // If above the range, move down
            if (pos.y - EntityData::PLAYER_REF.pos->y > COMN::resolution.y / 8 + 5)
                vel.y = -COMN::baseSpeed.y;
            else
                vel.y = COMN::baseSpeed.y;
        }
        else
            vel.y = 0;
    }

    Entity::tick(deltatime);
}
