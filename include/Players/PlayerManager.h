#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <string>
#include <map>
#include <sstream>

#include <SFML/Network/Packet.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include "Stats.h"
#include "Player.h"
#include "Database.h"

class PlayerManager {
public:
    static void Update();
    static void SendAll(sf::TcpSocket *client);

protected:

private:
    static unsigned int GetPlayerCount();
    static Player &GetPlayer (std::string id);
    static void RemovePlayer (std::string id);

    static void LoadPlayer (std::string playerName);

    template <typename T>
    static T StringTo(std::string input);

    static std::map<std::string, Player*> playerMap;
};

#endif // PLAYERMANAGER_H
