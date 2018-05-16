#include "PlayerManager.h"

std::map<std::string, Player*> PlayerManager::playerMap;
class NetworkManager;


void PlayerManager::Update()
{

}

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

void PlayerManager::SendAll(sf::TcpSocket *client)
{
    sf::Packet packet;
    for (std::map<std::string, Player*>::iterator it = playerMap.begin(); it != playerMap.end(); ++it) {
        packet << 12 << it->first << it->second->ToString();
        client->send(packet);
    }
}

void PlayerManager::LoadPlayer(std::string playerName)
{
    QueryResult qResult = Database::Execute("SELECT Players.* FROM Accounts, Players WHERE Accounts.Email=\"" + playerName + "\" AND Accounts.PlayerId = Players.PlayerID;");

    std::string parameters[] = { "Health", "Magic", "Stamina", "MaxHealth", "MaxMagic", "MaxStamina" };
    float mV, v;

    Stats stats;
    for (unsigned int i = 0; i < 3; i++) {
        v = StringTo<float>( qResult.QueryColumn(parameters[i]) );
        mV = StringTo<float>( qResult.QueryColumn(parameters[i+3]) );

        switch (i) {
            case 0:
                stats.health.SetStat(v, mV);
                break;
            case 1:
                stats.magic.SetStat(v, mV);
                break;
            case 2:
                stats.stamina.SetStat(v, mV);
                break;
        }

    }

    sf::Vector2u worldPos( StringTo<unsigned int> (qResult.QueryColumn("WorldX")), StringTo<unsigned int> (qResult.QueryColumn("WorldY")));
    sf::Vector2u cellPos( StringTo<unsigned int> (qResult.QueryColumn("CellX")), StringTo<unsigned int> (qResult.QueryColumn("CellY")));

    Player player(worldPos, cellPos);
}


template <typename T>
T PlayerManager::StringTo(std::string input)
{
    std::stringstream sStream;
    sStream << input;
    T output;
    sStream >> output;
    return output;
}
