#include "../Player.h"


Action* Player::actions = nullptr;

Player::Player(sf::Vector2f pos_, bool isScripted_, EntityScript *script_): Entity(pos_, EntityID::PLAYER, isScripted_, script_)
{
    Entity::EntityData::PlayerRef::pos = &pos;
    Entity::EntityData::PlayerRef::vel = &vel;
	EntityData::PlayerRef::entity = this;

    // Player faces wrong way after reset sometimes
    processActions();
}

void Player::tick(double deltatime) {
    processActions();

    applyFriction(deltatime, 0.6);
    vel.y = 0;

    // Horizontal Movement
    if (actions->flags.thrust)
        vel.x = (float)((left ? -1 :1) * COMN::baseSpeed.x * COMN::playerSpeedFactor);

    if (actions->flags.up)
        vel.y = (float)(-COMN::baseSpeed.y * COMN::playerSpeedFactor);
    else if (actions->flags.down)
        vel.y = (float)(COMN::baseSpeed.y * COMN::playerSpeedFactor);

    //std::cout << "PLAYER FLAGS: F: " << (short)actions->flags.fire << '\n';

    if (actions->flags.fire)
    {
        entityQueue.emplace(sf::Vector2f{pos.x - 4, pos.y}, EntityID::LASER);
        actions->flags.fire = false; // In theory this is redundant, yet here we are.
    }
    Entity::tick(deltatime);
    visual->forceUpdateSprite();
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    miniSprite->setTextureRect(sf::IntRect(80, 3, 3, 3));

    Entity::draw(target, states);
}

void Player::setActions(Action& actions_)
{
	actions = &actions_;
}

bool Player::collide(Entity *other) {
    return false;// Entity::collide(other);
}

bool Player::getDir() const {
    return left;
}

void Player::processActions() {
    auto& playerVisualData = visual->getPlayerData();

    if (actions->flags.left)
    {
        actions->flags.left = false;
        left                = !left;

        uint8_t index = playerVisualData.frameIndex + (left ? 2 : 0);

        playerVisualData.sprite->setTextureRect({
            VisualComponent::getBounds(ID).left + VisualComponent::getBounds(ID).width * index + index,
            VisualComponent::getBounds(ID).top,
            playerVisualData.bounds.width,
            playerVisualData.bounds.height
            });
    }
}

void Player::wrap() {
    float diff = pos.x - DisplayManager::getView().getCenter().x;
    float x    = pos.x;

    Entity::wrap();

    if (x != pos.x)
        DisplayManager::getView().setCenter(
                pos.x - diff,
                DisplayManager::getView().getCenter().y);

    pos.y = std::max<float>(pos.y, COMN::uiHeight);

    pos.y = std::min(pos.y, COMN::resolution.y - getBounds(EntityID::PLAYER).height);

    //std::cout << "PLAYER POS: " << pos.x << '\n';
}
