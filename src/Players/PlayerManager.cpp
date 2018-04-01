#include "PlayerManager.h"

std::map<std::string, Player*> PlayerManager::playerMap;

unsigned int PlayerManager::GetPlayerCount()
{
    return playerMap.size();
}

Player &PlayerManager::GetPlayer (std::string id)
{
    std::map<std::string, Player*>::iterator it;
    it = playerMap.find (id);
    return *it->second;
}

void PlayerManager::RemovePlayer(std::string id)
{
    std::map<std::string, Player*>::iterator it;
    it = playerMap.find(id);
    if (it != playerMap.end()) {
        playerMap.erase(it);
    }
}
