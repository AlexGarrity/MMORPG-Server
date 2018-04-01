#include "ObjectManager.h"

std::map<std::string, Entity*> ObjectManager::entityMap;
std::map<std::string, Entity*>::iterator ObjectManager::entityIterator;

//NetworkInstruction ObjectManager::networkInstruction;

void ObjectManager::Update()
{

    std::map<std::string, Entity*>::iterator it;

    for (it = entityMap.begin(); it != entityMap.end(); ++it) {
        it->second->Update();
    }
}

