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
    static  void setActions(Action& actions);
    virtual bool collide(Entity *other) override;
    void         fixX(float x_);
    void         setAstro(Entity* astro);
    bool         hasAstro() const;
    bool         getDir() const;

private:
	void         processActions();
    virtual void wrap() override;

    Entity* astronaut = nullptr;
    static Action* actions;
	bool left = false;
};