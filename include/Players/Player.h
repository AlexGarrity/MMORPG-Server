#ifndef PLAYER_H
#define PLAYER_H

#include <Entity.h>


class Player : public Entity
{
public:
    Player (sf::Vector2u wPosition, sf::Vector2u cPosition);
    virtual ~Player();

    void Update();
protected:

private:

};

#endif // PLAYER_H
