#include "Player.h"

Player::Player (sf::Vector2u wPosition, sf::Vector2u cPosition) : Entity (wPosition, cPosition)
{
    //ctor
}

Player::~Player()
{
    //dtor
}

void Player::Update()
{
    CheckVitals();
}

std::string Player::ToString()
{
    std::string result = "";
    result += std::to_string(worldPosition.x) + std::to_string(worldPosition.y) + std::to_string(cellPosition.x) + std::to_string(cellPosition.y);

    return result;
}
