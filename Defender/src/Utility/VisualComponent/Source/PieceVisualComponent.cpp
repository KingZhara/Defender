#include "../PieceVisualComponent.h"

sf::Sprite* PieceVisualComponent::piece = nullptr;

PieceVisualComponent::PieceVisualComponent(sf::IntRect bounds_, EntityID::ID ID_) : bounds(bounds_), VisualComponent(ID_) {}

void PieceVisualComponent::initialize(sf::Texture *tex_)
{
    piece = new sf::Sprite(*tex_, sf::IntRect(0, 0, 2, 2));
}

bool PieceVisualComponent::intersects(sf::FloatRect other)
{
    return false;
}

void PieceVisualComponent::draw(sf::RenderTarget &target,
        sf::RenderStates states) const
{
	piece->setTextureRect(bounds);
	patchDrawData(piece, states);
	target.draw(*piece, states);
}
