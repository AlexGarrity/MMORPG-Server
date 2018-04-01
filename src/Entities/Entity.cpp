#include "Entity.h"

Entity::Entity (sf::Vector2u wPosition, sf::Vector2u cPosition)
{
    worldPosition = wPosition;
    cellPosition = cPosition;
}

Entity::~Entity()
{
    //dtor
}

void Entity::Update()
{

}
