#pragma once
#include "VisualComponent.h"

class PieceVisualComponent : public VisualComponent
{
public:
	PieceVisualComponent(sf::IntRect bounds_, EntityID::ID ID_);

	static void  initialize(sf::Texture* tex_);
    virtual bool intersects(sf::FloatRect other) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	static sf::Sprite* piece;

	sf::IntRect bounds;
};

