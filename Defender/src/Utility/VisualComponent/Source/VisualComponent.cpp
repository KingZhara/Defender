#include "../VisualComponent.h"

#include "../../UserInterface/UserInterface.h"

sf::Texture* VisualComponent::tex = nullptr;

SpriteTableEntry VisualComponent::DATA_TABLE[EntityID::LENGTH] =
{
    // Player
    SpriteTableEntry{
            {0, 18, 15, 6}, // Sprite IntRect
            2, // Sprite frame count
            // Shader type
            // Custom frame length
        },
    // Astronaut
    SpriteTableEntry{
        {48, 0, 3, 8},
        1
    },
    // Bullet
    SpriteTableEntry{
        {13, 9, 3, 3},
        1
    },
    // Laser
    // @todo Replace later; blank data, will not use a sprite for drawing, custom image
    SpriteTableEntry{
        {19, 0, 8, 8},
        2
    },
    // Bomb
    SpriteTableEntry{
        {9, 9, 8, 8},
        2
    },
    // Lander
    SpriteTableEntry{
        {23, 9, 9, 8},
        4
    },
    // Mutant
    SpriteTableEntry{
        {0, 0, 10, 8},
        1,
        ShaderID::RAND_COL
    },
    // Baiter
    SpriteTableEntry{
        {63, 9, 11, 4},
        2
    },
    // Bomber
    SpriteTableEntry{
        {52, 0, 6, 7},
        4
    },
    // Pod
    SpriteTableEntry{
        {40, 0, 7, 7},
        1
    },
    // Swarmer
    SpriteTableEntry{
        {17, 9, 5, 4},
        1
    },
    // Particle
    SpriteTableEntry{
        {0, 0, 1, 0},
        1
    },
    // Piece
    SpriteTableEntry{
        {0, 0, 10, 8},
        1,
        ShaderID::RAND_COL
    },
    // Death animation piece
    SpriteTableEntry{
        {92, 0, 2, 2},
        1,
        ShaderID::DEATH_ANIM
    }
};

VisualComponent::VisualComponent(EntityID::ID ID_) : ID(ID_)
{
	// Set default position to 0,0
	pos = sf::Vector2f(0, 0);
}

void VisualComponent::setPosition(sf::Vector2f pos_)
{
	pos = pos_;
}

void VisualComponent::draw(sf::RenderTarget &target,
                           sf::RenderStates states) const
{
	std::cout << "Drawing ID: " << (short)ID << '\n';
    DATA_TABLE[ID].sprite->setPosition(pos);
	states.shader = UserInterface::getShader(DATA_TABLE[ID].shader);
	target.draw(*DATA_TABLE[ID].sprite, states);
}

bool VisualComponent::intersects(sf::FloatRect other)
{
	DATA_TABLE[ID].sprite->setPosition(pos);
	return DATA_TABLE[ID].sprite->getGlobalBounds().intersects(other);
}

bool VisualComponent::intersects(VisualComponent *other)
{
	DATA_TABLE[ID].sprite->setPosition(pos);
	return intersects(DATA_TABLE[other->ID].sprite->getGlobalBounds());
}

void VisualComponent::tick(double deltatime)
{
    // For every sprite
    for (uint8_t i = 0; i < EntityID::LENGTH; i++)
    {
		SpriteTableEntry& SPRITE_DATA = DATA_TABLE[i];

        if (SPRITE_DATA.frameCount > 1 && SPRITE_DATA.frameTimer.tick(deltatime))
        {
            // Everything is relative to current bounds because
            // the player needs to change textures
            sf::IntRect newBounds = SPRITE_DATA.sprite->getTextureRect();
            if (++SPRITE_DATA.frameIndex == SPRITE_DATA.frameCount)
            {
                SPRITE_DATA.frameIndex = 0;
                //std::cout << "Old Left: " << newBounds.left << " New Left: ";
                newBounds.left -= newBounds.width * (SPRITE_DATA.frameCount - 1) + SPRITE_DATA.frameCount - 1;
                //std::cout << newBounds.left << '\n';
            }
            else
            {
                newBounds.left += newBounds.width + 1;
                //std::cout << "ANIM BOUNDS: " << newBounds.left << ", " << newBounds.top << ", " << newBounds.width << ", " << newBounds.height << '\n';
            }

            SPRITE_DATA.sprite->setTextureRect(newBounds);
        }
    }
}

void VisualComponent::initialize(sf::Texture* tex_)
{
	tex = tex_;

	for (int i = 0; i < EntityID::LENGTH; ++i)
	{
		DATA_TABLE[i].sprite = new sf::Sprite(*tex_);
		DATA_TABLE[i].sprite->setTextureRect(DATA_TABLE[i].bounds);
	}
}

SpriteTableEntry & VisualComponent::getPlayerData()
{
	return DATA_TABLE[EntityID::PLAYER];
}

sf::IntRect VisualComponent::getBounds(EntityID::ID ID)
{
    return DATA_TABLE[ID].bounds;
}

void VisualComponent::patchDrawData(sf::Sprite * sprite, sf::RenderStates& states) const
{
    sprite->setPosition(pos);
	states.shader = UserInterface::getShader(DATA_TABLE[ID].shader);
}
