#include "../UserInterface.h"
#include "../../Timer.h"
#include "../../src/Entity/Enemy/Mutant.h"
#include "../HSV.h"


UserInterface::World UserInterface::world;
UserInterface::Stars UserInterface::stars;
UserInterface::Minimap UserInterface::minimap;
sf::Font UserInterface::font;
sf::Font UserInterface::otherFont;
sf::Text UserInterface::score;
sf::Text UserInterface::credits; // @todo find out if this is necessary...
sf::Shader* UserInterface::shiftingShader = nullptr;
sf::Shader* UserInterface::flashingShader = nullptr;
sf::RectangleShape UserInterface::World::border;
sf::Sprite UserInterface::World::background;
sf::Texture* UserInterface::World::bgTex = nullptr;
sf::Glsl::Vec3 UserInterface::brightColors[] =
{ // Data table from somewhere...
    {1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {1.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {1.0f, 0.5f, 0.0f}
};


uint8_t UserInterface::World::Component::generate(sf::Image &img, sf::Vector2u& pos, uint16_t size)
{
    bool invalid;
	std::cout << "\nSIZE: " << size << '\n';
    do
    {
        invalid = false;

        switch (type)
        {
        case Type::FLAT:
            // Flat draws 2x2; is invalid if at the top or the right side
            if (pos.y >= COMN::worldBgHeight - 1 || pos.x >= COMN::worldSize - 1)
            {
                invalid = true;
                type = Type::DOWN;
            }
            else
                size = std::min<uint16_t>(COMN::worldSize - pos.x, size - pos.x);
            break;

        case Type::UP:
            // This size is the upper bound
            size = pos.y < COMN::world_startY // If below the line
                    ? COMN::world_startY - pos.y // Take the Dy
                    : size - (pos.x + (pos.y - COMN::world_startY)); // Else, get combined delta; y from midline

            if (size == 0 && size - pos.x > 0)
            {
                invalid = true;
                type = Type::DOWN;
            }
            break;

        case Type::DOWN:
            size = pos.y > COMN::world_startY // if above the line
                    ? pos.y - COMN::world_startY// Take the Dx
                    : size - (pos.x + (COMN::world_startY - pos.y)); // Else, get combined delta; y from midline

            if (size == 0 && size - pos.x > 0)
            {
                invalid = true;
                type = Type::UP;
            }
            break;
        }
    } while (invalid);

    length = std::min<uint8_t>(length, static_cast<uint8_t>(std::min<uint16_t>(255, size)));

	std::cout << "LEN: " << (short)length << ", POS: (" << pos.x << ", " << pos.y << ")\n{\n";

    for (uint8_t i = 0; i < length; i++)
    {
        std::cout << "    ";
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
            std::cout << "(" << pos.x << ", " << pos.y << "), ";
            img.setPixel(pos.x + 1, pos.y, sf::Color::White);
            ++pos.x;
            ++i;
            break;
        }

        ++pos.x;
        std::cout << "(" << pos.x << ", " << pos.y << ") | " << "COUNT: " << (short)i << '\n';
        img.setPixel(pos.x, pos.y, sf::Color::White);
    }

    return length;
}

void UserInterface::World::generate()
{
	sf::Vector2u pos(0, COMN::world_startY);
    uint8_t genType = 2; // rand() % 3;
	uint16_t genLength = 0;
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
        piece = Component{
            static_cast<uint8_t>(rand() % 3),
            static_cast<uint8_t>(rand() % 1 + 1)
        };
        /*genLength = rand() % 800 + 201;
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
            std::cout << '\n' << genLength << '\n';*/
			genLength -= piece.generate(bgIntermediary, pos, COMN::worldSize/* - pos.x /*std::min(pos.x < genLength ? genLength - pos.x : genLength, COMN::worldSize - pos.x)*/);
        //}
        //std::cout << '\n' << pos.x << " < " << COMN::worldSize << '\n';
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
    font.setSmooth(false);
    otherFont.loadFromFile("res/defendercr.ttf");
    otherFont.setSmooth(false);

    // Shaders
    shiftingShader = new sf::Shader;
    flashingShader = new sf::Shader;

    shiftingShader->loadFromFile("res/shaders/replace.frag",
                                 sf::Shader::Type::Fragment);
    flashingShader->loadFromFile("./res/shaders/replace.frag",
        sf::Shader::Type::Fragment);

    flashingShader->setUniform("targetColor", sf::Glsl::Vec3{ 136.0f / 255.0f, 0.0f, 255.0f / 255.0f });
    shiftingShader->setUniform("targetColor", sf::Glsl::Vec3{ 136.0f / 255.0f, 0.0f, 255.0f / 255.0f });

    // World
    //UserInterface::World::generate();
}

const sf::Font &UserInterface::getFont() { return font; }
const sf::Font &UserInterface::getOtherFont() { return otherFont; }

sf::Shader * UserInterface::getShiftingShader() { return shiftingShader; }
sf::Shader * UserInterface::getFlashingShader() { return flashingShader; }

const void UserInterface::shaderTick(double deltatime)
{
    static Timer<double>      replaceType{ 1 / 8. };
	static bool               type = false;
    static HSV                shiftReplacement;


    if (replaceType.tick(deltatime))
    {
        type = !type;

        if (type)
            flashingShader->setUniform("replaceColor",
                brightColors[rand() % 8]);
        else
            flashingShader->setUniform("replaceColor", sf::Glsl::Vec3(0, 0, 0));
    }

    shiftReplacement.shift(10);

	shiftingShader->setUniform("replaceColor", shiftReplacement);
}

void UserInterface::drawBackground(sf::RenderTarget& rnd, sf::View& viewport)
{
	rnd.draw(world);
}

void UserInterface::drawForeground(sf::RenderTarget &, sf::View &) {}
