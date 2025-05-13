#pragma once
#include "Entity.h"
class Astronaut : public Entity
{
public:
	explicit Astronaut(sf::Vector2f pos_,
		bool         isScripted_ = false, EntityScript* script_ = nullptr);

    virtual ~Astronaut() override = default;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	virtual void tick(double deltatime) override;

	void setTargeted(bool v);
	bool targeted();
	bool isOnGround()   const;
    void setHolder(Entity* holder_);
	bool shouldDie();

private:
	bool isTargeted = false;
	bool held;
	bool onGround;
	uint8_t startFallHeight = 0;
};

