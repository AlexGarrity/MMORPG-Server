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

sf::Vector2u *Entity::GetPosition()
{
    sf::Vector2u positions[2] = {worldPosition, cellPosition};
    return positions;
}

bool Entity::CheckVitals() {
    return stats.health.value < 0 ? true : false;
}
