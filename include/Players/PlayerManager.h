#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include "Player.h"

#include <string>
#include <map>

class PlayerManager
{
public:
    static void Update();

protected:

private:

    static unsigned int GetPlayerCount();
    static Player &GetPlayer (std::string id);
    static void RemovePlayer(std::string id);

    static void LoadPlayer(std::string playerName);

    static std::map<std::string, Player*> playerMap;
};

#endif // PLAYERMANAGER_H
