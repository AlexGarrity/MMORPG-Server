#include "ObjectManager.h"

std::map<std::string, Entity*> ObjectManager::entityMap;
std::map<std::string, Entity*>::iterator ObjectManager::entityIterator;

//NetworkInstruction ObjectManager::networkInstruction;

void ObjectManager::Update()
{

    if (MapManager::GetMap(0)->GetEntityCount() == 0) {
        PlaceEntities();
    }

    std::map<std::string, Entity*>::iterator it;
    for (it = entityMap.begin(); it != entityMap.end(); ++it) {
        it->second->Update();
        if (!it->second->CheckVitals()) {
            sf::Vector2u *positions = it->second->GetPosition();
            unsigned int linearMapPosition = (MapManager::MapSize()[0] * positions[0].y) + positions[0].y;
            Map *currentMap = MapManager::GetMap(linearMapPosition);
            currentMap->RemoveEntity(positions[1].x, positions[1].y);
            entityMap.erase(it);

            delete currentMap;      //Pointer cleanup to avoid memory leaks
            delete[] positions;
        }
    }
}

bool ObjectManager::PlaceEntities()
{
    Logging::Log("Beginning to place entities", Logging::Severity::Debug);

    Map *currentMap = nullptr;

    Logging::Log("There are a total of " + std::to_string(MapManager::MapCount()) + " maps", Logging::Severity::Debug);

    for (unsigned int i = 0; i < MapManager::MapCount(); i++)
    {
        Logging::Log("Placing entities for map " + std::to_string(i), Logging::Severity::Debug);
        currentMap = MapManager::GetMap(i);
        unsigned int entityDifference = 0;
        unsigned int tileX = 0;
        unsigned int tileY = 0;
        bool validPlacement = false;

        entityDifference = 10 - currentMap->GetEntityCount();


        while (entityDifference > 0) {
            entityDifference = 10 - currentMap->GetEntityCount();   //Can be tweaked to balance the game
            Logging::Log("The entity difference between the target and current amount is " + std::to_string(entityDifference), Logging::Severity::Debug);
            validPlacement = false;

            while (!validPlacement) {
                unsigned int tileX = rand() % 50;
                unsigned int tileY = rand() % 50;

                Logging::Log("Trying to place entity at " + std::to_string(tileX) + ", " + std::to_string(tileY), Logging::Severity::Debug);
                validPlacement = currentMap->AddEntity(tileX, tileY);
            }

        }

    }

    Logging::Log("Placed entities", Logging::Severity::Debug);
    currentMap = nullptr;
    delete currentMap;
    return true;
}
