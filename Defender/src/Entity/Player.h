#pragma once
#include <algorithm>
#include <numbers>
#include "Entity.h"
#include "../Utility/Action.h"
#include "../Utility/DisplayManager.h"
#include "../Utility/common.h"

//@todo fix player sprite

class Player :
    public Entity
{
public:
	Player(sf::Vector2f pos_,
		bool         isScripted_ = false, EntityScript* script_ = nullptr);

    virtual ~Player() override = default;

	virtual void tick(double deltatime) override;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    static void setActions(Action& actions);
    virtual bool collide(Entity *other) override;
    void fixX(float x_)
    {
        visual->setPosition({ x_, pos.y });
    }

    void setAstro(Entity* astro)
    {
        astronaut = astro;
    }

    bool hasAstro()
    {
        return astronaut;
    }

    bool getDir() const;

private:
	void         processActions();
    virtual void wrap() override;

    Entity* astronaut = nullptr;
    static Action* actions;
	bool left = false;
};

// Go to user interface and look at the handling of the death animaation
//the death animation should have a staart function that is called inside of Entity manager upon[player state == respawning; initialization
//please ensure that the start method changes the boolean, and make sure that entity managers drawing included the death animation if it exists
//if both of those are true then please add debug statements to check activity inside of the user interface method, but ensure drawing works before shading