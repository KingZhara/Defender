#include "../UserInterface.h"

void UserInterface::World::generate()
{
}




// ############### USER INTERFACE ###############
void UserInterface::initialize()
{
	font.loadFromFile("res/defender.ttf");
	flashingShader.loadFromFile("res/shaders/flashing.frag", sf::Shader::Type::Fragment);
	shiftingShader.loadFromFile("res/shaders/shifting.frag", sf::Shader::Type::Fragment);
}

const sf::Font& UserInterface::getFont()
{
	return font;
}

const sf::Shader& UserInterface::getFlashingShader()
{
	return flashingShader;
}

const sf::Shader& UserInterface::getShiftingShader()
{
	return shiftingShader;
}

void UserInterface::drawBackground(sf::RenderTarget&, sf::View&)
{
}

void UserInterface::drawForeground(sf::RenderTarget&, sf::View&)
{
}
