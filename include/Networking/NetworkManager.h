#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "Logging.h"

#include "PlayerManager.h"
#include "ObjectManager.h"
#include "LoginServer.h"

#include <SFML/Network.hpp>

#include <vector>
#include <queue>
#include <string>
#include <algorithm>

struct NetworkInstruction {
public:
    NetworkInstruction();
    NetworkInstruction (unsigned char t, std::string s, std::string d)
    {
        type = t;
        subject = s;
        details = d;
    }

    unsigned char type;
    std::string subject;
    std::string details;
};

class InstructionParser
{
    public:
        static void Parse();

};

class NetworkManager
{
public:
    static void Initialise();
    static void Update();
    static void UpdateLoop();

    static void Heartbeat();

    static NetworkInstruction &GetNextInstruction();
    static bool DownQueueEmpty();
    static void AddInstructionUp (NetworkInstruction networkInstruction);
    static void AddInstructionUp (unsigned char t, std::string s, std::string d);
    static void AddInstructionDown (NetworkInstruction networkInstruction);
    static void AddInstructionDown (unsigned char t, std::string s, std::string d);

    static void Parse(NetworkInstruction nI);

protected:

private:
    static void CheckConnections();

    static sf::UdpSocket udpSocket;
    static unsigned short tcpPort;
    static unsigned short udpPort;

    static sf::TcpListener listener;
    static sf::SocketSelector socketSelector;

    static sf::Packet packet;

    static std::vector<sf::TcpSocket*> clientList;
    static std::queue<NetworkInstruction> downQueue;
    static std::queue<NetworkInstruction> upQueue;
};

#endif // NETWORKMANAGER_H
