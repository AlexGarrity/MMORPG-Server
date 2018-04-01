#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/System/Vector2.hpp>

class Entity
{
public:
    Entity (sf::Vector2u wPosition, sf::Vector2u cPosition);
    virtual ~Entity();

    virtual void Update();
protected:

private:
    sf::Vector2u worldPosition;
    sf::Vector2u cellPosition;
};

#endif // ENTITY_H
