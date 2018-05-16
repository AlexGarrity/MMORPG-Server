#ifndef PLAYER_H
#define PLAYER_H

#include <string>

#include "Stats.h"
#include "Entity.h"


class Player : public Entity
{
public:
    Player (sf::Vector2u wPosition, sf::Vector2u cPosition);
    virtual ~Player();

    void Update();
    std::string ToString();

protected:

private:

};

#endif // PLAYER_H
