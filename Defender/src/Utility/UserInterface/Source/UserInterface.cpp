#include "../UserInterface.h"


UserInterface::World   UserInterface::world;
UserInterface::Stars   UserInterface::stars;
UserInterface::Minimap UserInterface::minimap;
sf::Font               UserInterface::font;
sf::Text               UserInterface::score;
sf::Text               UserInterface::credits; // @todo find out if this is necessary...
sf::Shader*            UserInterface::flashingShader = nullptr;
sf::Shader*            UserInterface::shiftingShader = nullptr;
sf::RectangleShape     UserInterface::World::border;
sf::Image              UserInterface::World::background;

void UserInterface::World::generate()
{
}

void UserInterface::World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}

void UserInterface::Minimap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}

void UserInterface::Stars::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}

// ############### USER INTERFACE ###############
void UserInterface::initialize()
{
	font.loadFromFile("res/defender.ttf");
	flashingShader->loadFromFile("res/shaders/flashing.frag", sf::Shader::Type::Fragment);
	shiftingShader->loadFromFile("res/shaders/shifting.frag", sf::Shader::Type::Fragment);
}

const sf::Font& UserInterface::getFont()
{
	return font;
}

const sf::Shader* UserInterface::getFlashingShader()
{
	return flashingShader;
}

const sf::Shader* UserInterface::getShiftingShader()
{
	return shiftingShader;
}

void UserInterface::drawBackground(sf::RenderTarget&, sf::View&)
{
}

void UserInterface::drawForeground(sf::RenderTarget&, sf::View&)
{
}
