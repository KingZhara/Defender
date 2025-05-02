#include "../UserInterface.h"
#include "../../Timer.h"
#include "../../src/Entity/Enemy/Mutant.h"
#include "../HSV.h"
#include "../../../State/StageState.h"



UserInterface::World UserInterface::world;
UserInterface::Stars UserInterface::stars;
UserInterface::Minimap UserInterface::minimap;
sf::Font* UserInterface::font;
sf::Font* UserInterface::otherFont;
sf::Text UserInterface::score;
sf::Text UserInterface::credits; // @todo find out if this is necessary...
sf::Shader* UserInterface::shiftingShader = nullptr;
sf::Shader* UserInterface::flashingShader = nullptr;
sf::Shader* UserInterface::williamsShader = nullptr;
sf::RectangleShape UserInterface::World::border;
sf::Sprite UserInterface::World::background;
sf::Texture* UserInterface::World::bgTex = nullptr;
HSV UserInterface::shiftReplacement{};
Color<float> UserInterface::brightColors[] =
{ // Data table from somewhere...
    {1.0f * 255.0f, 0.0f * 255.0f, 0.0f * 255.0f},
    {0.0f * 255.0f, 1.0f * 255.0f, 0.0f * 255.0f},
    {1.0f * 255.0f, 1.0f * 255.0f, 0.0f * 255.0f},
    {0.0f * 255.0f, 0.0f * 255.0f, 1.0f * 255.0f},
    {1.0f * 255.0f, 0.0f * 255.0f, 1.0f * 255.0f},
    {0.0f * 255.0f, 1.0f * 255.0f, 1.0f * 255.0f},
    {1.0f * 255.0f, 1.0f * 255.0f, 1.0f * 255.0f},
    {1.0f * 255.0f, 0.5f * 255.0f, 0.0f * 255.0f}
};


uint8_t UserInterface::World::Component::generate(sf::Image &img, sf::Vector2u& pos, uint16_t size)
{
    bool invalid;
	std::cout << "\nSIZE: " << size << '\n';
    //length = rand() % 10;
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
            if (pos.y - 1 == 0)
                return i;
            img.setPixel(++pos.x, --pos.y, brightColors[7]);
            //++pos.y;
            break;

        case Type::DOWN:
            if (pos.y + 1 > COMN::worldBgHeight)
                return i;
            img.setPixel(++pos.x, ++pos.y, brightColors[7]);
           // --pos.y;
            break;

        case Type::FLAT:
            // [  ##]
            // [##  ]
            std::cout << "(" << pos.x << ", " << pos.y << "), ";
            img.setPixel(++pos.x, pos.y - 1, brightColors[7]);
            img.setPixel(++pos.x, pos.y, brightColors[7]);
            //++pos.y;
            //++pos.x;
            ++i;
            break;
        }

        //++pos.x;
        std::cout << "(" << pos.x << ", " << pos.y << ") | " << "COUNT: " << (short)i << '\n';
        //img.setPixel(pos.x, pos.y, sf::Color::White);
    }

    return length;
}

void UserInterface::World::generate()
{
	sf::Vector2u pos(0, COMN::world_startY);
    uint8_t genType = rand() % 3;
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
        //piece = Component{
        //    static_cast<uint8_t>(rand() % 3),
        //    static_cast<uint8_t>(rand() % 1 + 1)
        //};
        genType = rand() % 3;
        genLength = (rand() % 800 + 201) % (COMN::worldSize - pos.x + 1);
        while (genLength > 0)
        {

            switch (genType)
            {
            case 0: // Mountains
                piece = Component{
                    static_cast<uint8_t>(rand() % 3),
                    static_cast<uint8_t>(20 + rand() % (40 - 20))
                };
				break;
            case 1: // Minor mountains
                piece = Component{
                    static_cast<uint8_t>(rand() % 3),
                    static_cast<uint8_t>(5 + rand() % (15 - 5))
                };
                break;
            case 2: // Rough
                piece = Component{
                    static_cast<uint8_t>(rand() % 3),
                    static_cast<uint8_t>(1 + rand() % (5 - 1))
                };
                break;
            }
            std::cout << '\n' << genLength << '\n';

			piece.length = std::min<uint8_t>(piece.length, static_cast<uint8_t>(std::min<uint16_t>(255, genLength)));
			genLength -= std::min<uint16_t>(genLength, piece.generate(bgIntermediary, pos, COMN::worldSize/* - pos.x /*std::min(pos.x < genLength ? genLength - pos.x : genLength, COMN::worldSize - pos.x)*/));
        }
        std::cout << '\n' << pos.x << " < " << COMN::worldSize << '\n';
    }

	bgTex->loadFromImage(bgIntermediary);
    bgTex->setRepeated(true);
	background.setTexture(*bgTex);
    background.setPosition(-COMN::worldSize, COMN::resolution.y - background.getGlobalBounds().height);
    background.setTextureRect(sf::IntRect(0, 0, COMN::worldSize * 3, background.getGlobalBounds().height));
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
    font = new sf::Font;
    otherFont = new sf::Font;

    font->loadFromFile("res/defendermono.ttf");
    font->setSmooth(false);
    otherFont->loadFromFile("res/defendercr.ttf");
    otherFont->setSmooth(false);

    // Shaders
    shiftingShader = new sf::Shader;
    flashingShader = new sf::Shader;
    williamsShader = new sf::Shader;

    shiftingShader->loadFromFile("res/shaders/replace.frag",
        sf::Shader::Type::Fragment);
    flashingShader->loadFromFile("./res/shaders/replace.frag",
        sf::Shader::Type::Fragment);
    williamsShader->loadFromFile("./res/shaders/replace.frag",
        sf::Shader::Type::Fragment);

    flashingShader->setUniform("targetColor", sf::Glsl::Vec3{ 136.0f / 255.0f, 0.0f, 255.0f / 255.0f });
    shiftingShader->setUniform("targetColor", sf::Glsl::Vec3{ 136.0f / 255.0f, 0.0f, 255.0f / 255.0f });
    williamsShader->setUniform("targetColor", sf::Glsl::Vec3{ 136.0f / 255.0f, 0.0f, 255.0f / 255.0f });

    // World
    UserInterface::World::generate();
}

const sf::Font &UserInterface::getFont() { return *font; }
const sf::Font &UserInterface::getOtherFont() { return *otherFont; }

sf::Shader * UserInterface::getShiftingShader() { return shiftingShader; }
sf::Shader * UserInterface::getFlashingShader() { return flashingShader; }
sf::Shader * UserInterface::getWilliamsShader() { return williamsShader; }

sf::Shader * UserInterface::getShader(ShaderID::ID ID)
{
    switch (ID)
    {
    case ShaderID::NONE:
        return nullptr;

    case ShaderID::HUE_SHIFT:
		return shiftingShader;

    case ShaderID::RAND_COL:
        return flashingShader;

    case ShaderID::STARS:
        return nullptr;//starsShader; @todo add star shader?

    case ShaderID::WILLIAMS:
        return williamsShader;
    }

	return nullptr;
}

const void UserInterface::shaderTick(double deltatime)
{
    static Timer<double>      replaceType{ 1 / 8. };
	static bool               type = false;
    static Timer<double>      replaceWilliams{ 1. };
	static bool               williamsType = false;

    if (replaceType.tick(deltatime))
    {
        type = !type;

        if (type)
            flashingShader->setUniform("replaceColor",
                brightColors[rand() % 8]);
        else
            flashingShader->setUniform("replaceColor", sf::Glsl::Vec3(0, 0, 0));
    }

    if (replaceWilliams.tick(deltatime))
    {
        williamsType = !williamsType;

        williamsShader->setUniform("replaceColor", williamsType ? sf::Glsl::Vec3(1, 1, 0) : sf::Glsl::Vec3(1, 0, 0));
    }

    shiftReplacement.shift(6);

	shiftingShader->setUniform("replaceColor", shiftReplacement);
}

void UserInterface::drawBackground(sf::RenderTarget& rnd, sf::View& viewport)
{
	rnd.draw(world);
}

void UserInterface::drawForeground(sf::RenderTarget& target, sf::View& view) 
{
    // Needs to reset view to highscore
    sf::View oldView = target.getView();

    sf::View defView = oldView;
    defView.setCenter(defView.getSize().x / 2.f, defView.getSize().y / 2.f);
    target.setView(defView);


    // Need to cover stuff from gameplay like projectiles ---------------------------------------------
    sf::RectangleShape clearUI;
    clearUI.setFillColor(sf::Color::Black);
    clearUI.setSize(sf::Vector2f(COMN::resolution.x, COMN::uiHeight));
    target.draw(clearUI);


    // UI dividers
    sf::RectangleShape uiDivider;
    uiDivider.setPosition(0, COMN::uiHeight);
    uiDivider.setSize(sf::Vector2f(COMN::resolution.x, 2));
    uiDivider.setFillColor(sf::Color::Blue);
    target.draw(uiDivider);

    sf::RectangleShape minimapDivider;
    minimapDivider.setPosition(82, 1);
    minimapDivider.setSize(sf::Vector2f((COMN::resolution.x / 2.f - 82) * 2, COMN::uiHeight - 1));
    minimapDivider.setFillColor(sf::Color::Transparent);
    minimapDivider.setOutlineColor(sf::Color::Blue);
    minimapDivider.setOutlineThickness(2);
    target.draw(minimapDivider);


    // Draw player lives ---------------------------------------------------------------------
    sf::IntRect  playerUI = { 64, 18, 10, 4 };
    sf::Vector2i playerUIPos = { 18, 12 };

    sf::RectangleShape lifeDisplay;
    lifeDisplay.setTexture(DisplayManager::getTexture());
    lifeDisplay.setTextureRect(playerUI);
    lifeDisplay.setSize(sf::Vector2f(playerUI.getSize()));

    for (int i = 0; i < std::min(StageState::getPlayerLives() - 1, 3); i++)
    {
        lifeDisplay.setPosition(playerUIPos.x + (playerUI.width + 2) * i, playerUIPos.y);
        target.draw(lifeDisplay);
    }

    // Draw bomb count ---------------------------------------------------------------------
    sf::IntRect  bombUI = { 75, 18, 6, 4 };
    sf::Vector2i bombUIPos = { 70, 19 };

    sf::RectangleShape bombDisplay;
    bombDisplay.setTexture(DisplayManager::getTexture());
    bombDisplay.setTextureRect(bombUI);
    bombDisplay.setSize(sf::Vector2f(bombUI.getSize()));

    for (int i = 0; i < std::min(StageState::getPlayerBombs(), 3); i++)
    {
        bombDisplay.setPosition(bombUIPos.x, bombUIPos.y + (bombUI.height + 1) * i);
        target.draw(bombDisplay);
    }


    // draw screen view markers -------------------------------------------------
    constexpr int screenMarkerWidth = 15;

    sf::RectangleShape screenMarkerMain;
    screenMarkerMain.setSize(sf::Vector2f(screenMarkerWidth, 2));
    screenMarkerMain.setFillColor(sf::Color(0xBFBFBFFF));


    sf::RectangleShape screenMarkerSide;
    screenMarkerSide.setSize(sf::Vector2f(1, 4));
    screenMarkerSide.setFillColor(sf::Color(0xBFBFBFFF));


    constexpr float minimapScale = 0.05;

    // top marker
    screenMarkerMain.setPosition(COMN::resolution.x / 2 - screenMarkerWidth / 2.f, 0);

    target.draw(screenMarkerMain);

    screenMarkerSide.setPosition(screenMarkerMain.getPosition().x, 0);
    target.draw(screenMarkerSide);

    screenMarkerSide.setPosition(screenMarkerMain.getPosition().x + screenMarkerWidth, 0);
    target.draw(screenMarkerSide);

    // bottom marker
    screenMarkerMain.setPosition(screenMarkerMain.getPosition().x, COMN::uiHeight);
    target.draw(screenMarkerMain);

    screenMarkerSide.setPosition(screenMarkerMain.getPosition().x, COMN::uiHeight - 2);
    target.draw(screenMarkerSide);

    screenMarkerSide.setPosition(screenMarkerMain.getPosition().x + screenMarkerWidth, COMN::uiHeight - 2);
    target.draw(screenMarkerSide);

    // Show right aligned score ------------------------------------------------------------
    // Same position as in highscore
    sf::Text scoreTxt;
    scoreTxt.setFont(UserInterface::getFont());
    scoreTxt.setCharacterSize(16);
    scoreTxt.setFillColor(sf::Color(COMN::ShaderTarget));
    scoreTxt.setString(std::to_string(StageState::getScore()));
    scoreTxt.setOrigin(scoreTxt.getGlobalBounds().width, 0);
    scoreTxt.setPosition(63, 14);
    target.draw(scoreTxt);


    // Draw minimap
    const EntityManager& entityManager = StageState::getEntityManager();
    sf::Vector2f playerPos = entityManager.getPlayerPos();


    sf::RectangleShape player;
    player.setSize({ 1, 1 });
    player.setPosition(
        // center player
        COMN::resolution.x / 2.f + 
        // get the player pos - view center
        (playerPos.x - DisplayManager::getView().getCenter().x) / 20.f
        + player.getSize().x / 2.f,

        // convert game pos to ui pos
        (playerPos.y - COMN::uiHeight) / (COMN::resolution.y - COMN::uiHeight)
        * (COMN::uiHeight - 4) + 4 - player.getSize().y);

    target.draw(player);

    // Reset view
    target.setView(oldView);
}
