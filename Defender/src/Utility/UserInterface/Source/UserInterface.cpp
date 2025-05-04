#include "../UserInterface.h"
#include "../../Timer.h"
#include "../HSV.h"
#include "../../DisplayManager.h"



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
UserInterface::DeathReplacement UserInterface::deathReplacement {};
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
sf::RenderTexture* UserInterface::miniTarget = nullptr;
sf::Sprite* UserInterface::miniSprite = nullptr;
const sf::Vector2f UserInterface::UIBounds::minimapPosConversion = sf::Vector2f{ (float)((double)MINIMAP_WIDTH / (double)COMN::worldSize), (float)((double)MINIMAP_HEIGHT / COMN::resolution.y) };


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

	miniTarget = new sf::RenderTexture;
    miniTarget->create(UIBounds::MINIMAP_WIDTH, UIBounds::MINIMAP_HEIGHT);
	miniTarget->setSmooth(false);
	miniTarget->setRepeated(true);

	miniSprite = new sf::Sprite(miniTarget->getTexture());

	miniSprite->setScale(1, -1);

	miniSprite->setPosition(UIBounds::MINIMAP_X, UIBounds::MINIMAP_Y);

	miniSprite->setTextureRect(sf::IntRect(0, 0, UIBounds::MINIMAP_WIDTH*3, UIBounds::MINIMAP_HEIGHT));
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

    if (deathReplacement.active)
    {
        // If not passed white, tick
        // if passed white
			// if not passed hue, shift
		    // if passed, set passed to true
                // if v == 0, set active to false
                // else, -= 1*deltatime

        if (!deathReplacement.passedWhite)
            deathReplacement.passedWhite = deathReplacement.whiteTime.tick(deltatime);
        else
        {
            if (!deathReplacement.passedHue)
            {
				deathReplacement.color.shift(1);

                if (HSV::hasPassedHue(HSV(0.6, 1., 1.), deathReplacement.color, 0))
                    deathReplacement.passedHue = true;
            }
            else
            {
				if (deathReplacement.color.getV() == 0)
				{
                    // Reset
                    deathReplacement.color = HSV(0.6, 1., 1.);
					deathReplacement.active = false;
					deathReplacement.passedHue = false;
                    deathReplacement.passedWhite = false;
				}
				else
					deathReplacement.color.dim(1. * deltatime);
            }
        }
    }
}

void UserInterface::drawBackground(sf::RenderTarget& rnd, sf::View& viewport)
{
	rnd.draw(world);
    // In theory this would also draw other things listed here
}

void UserInterface::drawForeground(sf::RenderTarget& target, sf::View& view, EntityManagerData& data) 
{
    static bool initialized = false;
    static sf::RectangleShape cover;
	static sf::RectangleShape divider;
    static sf::RenderTexture scoreTarget;
	static sf::Sprite scoreSprite(scoreTarget.getTexture());
    static sf::Text scoreTxt;
    static sf::View uiView;

    if (!initialized)
    {
		cover.setFillColor(sf::Color::Black);
        divider.setOutlineThickness(2);
		divider.setOutlineColor(sf::Color::Blue); // Needs to be changed
        divider.setFillColor(sf::Color::Transparent);
        scoreTxt.setFont(UserInterface::getFont());
        scoreTxt.setCharacterSize(16);

        scoreTarget.create(UIBounds::MINIMAP_X - 18, 7);

        scoreSprite.setScale(1, -1);
		scoreSprite.setPosition(0, 26);

        uiView = target.getDefaultView();
        uiView.setSize(COMN::resolution.x, COMN::resolution.y); // Set size of view (in world units)
        uiView.setCenter(COMN::resolution.x / 2.f, COMN::resolution.y / 2.f); // Center the view in world space


		initialized = true;
    }

    // Temporary for drawing HUD elements
    //view.setCenter(view.getSize().x / 2, view.getSize().y / 2);
    target.setView(uiView);

    // Cover gameplay for UI
    cover.setSize(sf::Vector2f(COMN::resolution.x, COMN::uiHeight));
    target.draw(cover);

    // Draw minimap
    sf::RenderStates states = sf::RenderStates::Default;
    states.shader = flashingShader;

    miniSprite->setPosition(UIBounds::MINIMAP_X - UIBounds::MINIMAP_WIDTH + data.minimapOffset, miniSprite->getPosition().y);
    target.draw(*miniSprite, states);
    miniTarget->clear();

    // Cover overflow
    cover.setSize({ UIBounds::MINIMAP_X - 1, COMN::uiHeight });
    target.draw(cover);
	cover.setPosition(COMN::resolution.x - (UIBounds::MINIMAP_X - 1), 0);
    target.draw(cover);

    // UI divider
    divider.setPosition(0, 0);
    divider.setSize(sf::Vector2f(COMN::resolution.x, COMN::uiHeight - 2));
    target.draw(divider);

    // Draw minimap box
    divider.setPosition(UIBounds::MINIMAP_X, 1);
    divider.setSize(sf::Vector2f(UIBounds::MINIMAP_WIDTH, UIBounds::MINIMAP_HEIGHT));
    target.draw(divider);


    // Draw player lives ---------------------------------------------------------------------
    repeatElement(
        { 64, 18, 10, 4 }, 
        { 18, 12 }, 
        { 12, 0 }, 
        std::min<uint8_t>(5, data.extraLives), target);

    // Draw bomb count ---------------------------------------------------------------------

    repeatElement(
        { 75, 18, 6, 4 }, 
        { 70, 19 }, 
        { 0, 4 }, 
        std::min<uint8_t>(3, data.smartBombs), target);


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


    //-18, 24

    
    scoreTxt.setString(std::to_string(data.score));
    //scoreTxt.setOrigin(scoreTxt.getGlobalBounds().width, 0);
    scoreTxt.setPosition(UIBounds::MINIMAP_X - 18 - scoreTxt.getGlobalBounds().width, 0);
    scoreTarget.draw(scoreTxt);

	states.shader = shiftingShader;
    target.draw(scoreSprite, states);

    // Death Animation
    // Validate stage state
    // Validate entity manager
	// Validate death replacement


    // Reset view
    target.setView(view);
	//view.setCenter(viewResetCenter);
}

void UserInterface::repeatElement(sf::IntRect texBounds,
        sf::Vector2f pos,
        sf::Vector2f diff,
        uint8_t count, 
        sf::RenderTarget& target)
{
    static sf::Sprite toDraw(*DisplayManager::getTexture());

	toDraw.setTextureRect(texBounds);
	//toDraw.setPosition(pos);

    for (uint8_t i = 0; i < count; i++)
    {
		toDraw.setPosition(pos.x + diff.x * i, pos.y + diff.y * i);
		target.draw(toDraw);
    }
}
