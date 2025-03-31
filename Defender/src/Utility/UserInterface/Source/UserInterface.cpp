#include "../UserInterface.h"
#include "../../Timer.h"
#include "../../src/Entity/Enemy/Mutant.h"


UserInterface::World UserInterface::world;
UserInterface::Stars UserInterface::stars;
UserInterface::Minimap UserInterface::minimap;
sf::Font UserInterface::font;
sf::Text UserInterface::score;
sf::Text UserInterface::credits; // @todo find out if this is necessary...
sf::Shader *UserInterface::shiftingShader = nullptr;
sf::RectangleShape UserInterface::World::border;
sf::Sprite UserInterface::World::background;
sf::Texture* UserInterface::World::bgTex = nullptr;


uint8_t UserInterface::World::Component::generate(sf::Image &img, sf::Vector2u& pos, uint16_t maxDx)
{
    // Type dictates direction
    // All lengths represent a change in position by 
    const uint16_t maxDy = std::min<uint16_t>(
            (COMN::worldSize - (pos.x + pos.y)) / 2, 
		    type == Type::UP
                     ? COMN::worldBgHeight - pos.y
                     : pos.y);

    maxDx = std::min<uint16_t>(255, maxDx);

	//std::cout << "MAX DX: " << (short)maxDx << ", MAX DY: " << maxDy << " | POS: (" << pos.x << ", " << pos.y << "), OFFSET: " << pos.x + pos.y << '\n';

    std::cout << (COMN::worldSize - (pos.x + pos.y)) / 2 << ", " << (type == Type::UP ? COMN::worldBgHeight - pos.y : pos.y) << '\n';

    length = std::min<uint8_t>(length, std::min<uint8_t>(maxDx, maxDy));

    if (maxDy == 0)
    {
        if (pos.y > COMN::worldBgStart)
        {
			type = Type::DOWN;
			length = pos.y - COMN::worldBgStart;
        }
		else if (pos.y < COMN::worldBgStart)
		{
			type = Type::UP;
			length = COMN::worldBgStart - pos.y;
		}
        else
        {
			length = maxDx;
        }
    }

    if (length == 0)
        return length;

    if (type == Type::FLAT)
        length /= 2;

    //std::cout << "LEN: " << (short)length << ", POS: (" << pos.x << ", " << pos.y << ")\n{\n";

    for (uint8_t i = 0; i < length; i++)
    {
        //std::cout << "    ";
        switch (type)
        {
        case Type::UP:
            ++pos.y;
            break;

        case Type::DOWN:
            --pos.y;
            break;

        case Type::FLAT:
            // [  ##]
            // [##  ]
            //std::cout << "(" << pos.x << ", " << pos.y << "), ";
            img.setPixel(pos.x + 1, pos.y, sf::Color::White);
            ++pos.x;
            ++i;
            break;
        }

        ++pos.x;
        //std::cout << "(" << pos.x << ", " << pos.y << ") | " << "COUNT: " << (short)i << '\n';
        img.setPixel(pos.x, pos.y, sf::Color::White);
    }

	//std::cout << "}\n\n";

    return length;
}

void UserInterface::World::generate()
{
	sf::Vector2u pos(0, COMN::worldBgStart);
    uint8_t genType = 2; // rand() % 3;
	uint16_t genLength;
    Component piece{0, 0};
    sf::Image bgIntermediary;
	bgTex = new sf::Texture;

    bgIntermediary.create(COMN::worldSize, COMN::worldBgHeight + 2, sf::Color::Black);

    //for (uint8_t i = 0; i < 90; i++)
    //{
    //    std::cout << "(" << 0 << ", " << (short)i << ")\n";
    //    bgIntermediary.setPixel(0, i, sf::Color::White);

    //}

    while (pos.x < COMN::worldSize)
    {
        genLength = rand() % 800 + 201;
        while (genLength > 0)
        {
            switch (genType)
            {
            case 0: // Mountains
				break;
            case 1: // Minor mountains
                break;
            case 2: // Rough
                piece = Component{
                    static_cast<uint8_t>(rand() % 3),
                    static_cast<uint8_t>(rand() % 10 + 1)
                };
                break;
            }
            std::cout << '\n' << genLength << '\n';
			genLength -= piece.generate(bgIntermediary, pos, std::min(pos.x < genLength ? genLength - pos.x : genLength, COMN::worldSize - pos.x));
        }
        std::cout << '\n' << pos.x << " < " << COMN::worldSize << '\n';
    }

	bgTex->loadFromImage(bgIntermediary);
	background.setTexture(*bgTex);
}

void UserInterface::World::draw(sf::RenderTarget &target,
                                sf::RenderStates  states) const
{
	target.draw(background, states);
	//target.draw(border, states);
}

void UserInterface::Minimap::draw(sf::RenderTarget &target,
                                  sf::RenderStates  states) const {}

void UserInterface::Stars::draw(sf::RenderTarget &target,
                                sf::RenderStates  states) const {}

// ############### USER INTERFACE ###############
void UserInterface::initialize()
{
    font.loadFromFile("res/defendermono.ttf");

    shiftingShader = new sf::Shader;

    shiftingShader->loadFromFile("res/shaders/shifting.frag",
                                 sf::Shader::Type::Fragment);

    UserInterface::World::generate();
    Mutant::initShader();
}

const sf::Font &UserInterface::getFont() { return font; }

sf::Shader *UserInterface::getShiftingShader() { return shiftingShader; }

const void UserInterface::shaderTick(double deltatime) {}

void UserInterface::drawBackground(sf::RenderTarget& rnd, sf::View& viewport)
{
	rnd.draw(world);
}

void UserInterface::drawForeground(sf::RenderTarget &, sf::View &) {}
