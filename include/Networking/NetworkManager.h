#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <vector>
#include <queue>
#include <string>
#include <cstring>
#include <algorithm>
#include <utility>

#include "Logging.h"

#include "ObjectManager.h"
#include "LoginServer.h"

#include <SFML/Network.hpp>


struct NetworkInstruction {
public:
    NetworkInstruction() {

    }
    NetworkInstruction (sf::Uint8 t, sf::String s, sf::String d)
    {
        type = t;
        subject = s;
        details = d;
    }

    bool Send (sf::TcpSocket &client) {
        sf::Packet packet;
        packet << type << subject << details;
        if (client.send(packet) == sf::Socket::Done) {
            return true;
        }
        return false;
    }

    bool Receive (sf::TcpSocket &client) {
        sf::Packet packet;
        if (client.receive(packet) == sf::Socket::Done) {
            packet >> type >> subject >> details;
            return true;
        }
        return false;
    }

    void Clear() {
        type = 00;
        subject = "";
        details = "";
    }

    sf::Uint8 type;
    sf::String subject;
    sf::String details;
};

class InstructionParser
{
    public:
        static void Parse();

};

class NetworkManager
{
public:
    static bool Initialise();
    static void Update();
    static void UpdateLoop();

    static void Heartbeat();
    static void RemoveClients();

    static std::pair<NetworkInstruction*, sf::TcpSocket*> &GetNextInstruction();

    static bool DownQueueEmpty();
    static bool UpQueueEmpty();

    static void AddInstructionUp (NetworkInstruction *networkInstruction, sf::TcpSocket *client);
    static void AddInstructionUp (unsigned char t, std::string s, std::string d, sf::TcpSocket *client);
    static void AddInstructionDown (NetworkInstruction *networkInstruction, sf::TcpSocket *client);
    static void AddInstructionDown (unsigned char t, std::string s, std::string d, sf::TcpSocket *client);

    static void Parse(std::pair<NetworkInstruction*, sf::TcpSocket*> &instruction);

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
    static std::queue<std::pair<NetworkInstruction*, sf::TcpSocket*>> downQueue;
    static std::queue<std::pair<NetworkInstruction*, sf::TcpSocket*>> upQueue;
};

#endif // NETWORKMANAGER_H
