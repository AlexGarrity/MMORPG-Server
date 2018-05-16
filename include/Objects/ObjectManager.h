#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <SFML/System/Vector2.hpp>

#include <map>
#include <string>
#include <random>

#include "Entity.h"
#include "Player.h"
#include "MapManager.h"
#include "PlayerManager.h"
#include "NetworkManager.h"



class ObjectManager
{
public:
    static void Update();

protected:

private:
    static bool PlaceEntities();
    static void PlaceEntity(std::string entityType);

    static std::map<std::string, Entity*> entityMap;
    static std::map<std::string, Entity*>::iterator entityIterator;

    // static std::queue<ObjectUpdate> objectUpdateQueue();
};

#endif // OBJECTMANAGER_H
