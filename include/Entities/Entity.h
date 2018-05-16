#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/System/Vector2.hpp>

#include "Stats.h"

class Entity
{
public:
    Entity (sf::Vector2u wPosition, sf::Vector2u cPosition);
    virtual ~Entity();

    virtual void Update();

    sf::Vector2u *GetPosition();
    bool CheckVitals();
protected:
    sf::Vector2u worldPosition;
    sf::Vector2u cellPosition;

    sf::Vector2u worldTarget;
    sf::Vector2u cellTarget;

    Stats stats;
private:

};


#endif // ENTITY_H
