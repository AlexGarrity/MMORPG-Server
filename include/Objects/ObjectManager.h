#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <SFML/System/Vector2.hpp>

#include <map>
#include <string>

#include "Entity.h"
#include "Player.h"
#include "PlayerManager.h"
#include "NetworkManager.h"



class ObjectManager
{
public:
    static void Update();

protected:

private:
    static void PlaceEntities();

    static std::map<std::string, Entity*> entityMap;
    static std::map<std::string, Entity*>::iterator entityIterator;

    // static std::queue<ObjectUpdate> objectUpdateQueue();
};

#endif // OBJECTMANAGER_H
