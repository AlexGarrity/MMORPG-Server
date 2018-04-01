#include "NetworkManager.h"

sf::TcpListener NetworkManager::listener;                   //Forward declaration as these are all static members...
sf::SocketSelector NetworkManager::socketSelector;

sf::Packet NetworkManager::packet;

std::vector<sf::TcpSocket*> NetworkManager::clientList;
std::queue<NetworkInstruction> NetworkManager::downQueue;
std::queue<NetworkInstruction> NetworkManager::upQueue;

void InstructionParser::Parse()
{
    Logging::Log("Parsing instructions on the down stream", Logging::Debug);
    NetworkInstruction *networkInstruction = nullptr;
    if (NetworkManager::DownQueueEmpty()) {
        Logging::Log("Turn out there aren't any", Logging::Debug);
    }
    while (NetworkManager::DownQueueEmpty() != true) {
        networkInstruction = &NetworkManager::GetNextInstruction();
        unsigned char instructionType = networkInstruction->type / 10;
        Logging::Log("Have instruction of major type: " + instructionType, Logging::Debug);
        switch (instructionType) {
            case 0:
                //Login Server Instructions
                NetworkManager::Parse(*networkInstruction);
                break;
            case 1:
                //Player Instructions
                //PlayerManager::Parse(*networkInstruction);
                break;
            case 2:
                //Entity Instructions
                //ObjectManager::Parse(*networkInstruction);
                break;
        }
    }
    delete networkInstruction;
}

void NetworkManager::Parse(NetworkInstruction nI) {
    switch (nI.type) {
        case 0:

        break;
        case 1:
        Logging::Log("Got disconnect request from: " + nI.subject, Logging::Info);
            sf::IpAddress client = nI.subject;
            LoginServer::Logout(nI.details);

            std::vector<sf::TcpSocket*>::iterator vectorIndex;
            for (vectorIndex = clientList.begin(); vectorIndex != clientList.end(); ++vectorIndex) {
                if ((*vectorIndex)->getRemoteAddress() == client) {
                    (*vectorIndex)->disconnect();
                    clientList.erase(vectorIndex);
                    Logging::Log("Disconnected a client: " + nI.subject, Logging::Info);
                    break;
                }
            }

        break;
    }
}

void NetworkManager::Initialise()
{
    if (listener.listen(6401) != sf::Socket::Done) {                //If the listener fails to start listening on 6401...
        Logging::Log("Listener failed to listen on port " + std::to_string(listener.getLocalPort()), Logging::Important);     //Error message
    } else {                                                        //Otherwise...
        Logging::Log("Listener is now listening on port " + std::to_string(listener.getLocalPort()), Logging::Important);     //Success message
    }

    socketSelector.add (listener);      //Add the TCP Listener to the Socket Selector

    Logging::Log("The remote address of the server is " + sf::IpAddress::getPublicAddress().toString(), Logging::Info);
    Logging::Log("The local address of the server is " + sf::IpAddress::getLocalAddress().toString(), Logging::Info);
}

void NetworkManager::Update()
{
    if (socketSelector.wait(sf::seconds(0.1))) {           //Wait until a socket is ready to receive, with a timeout of 0.1 seconds
        if (socketSelector.isReady (listener) ) {       //If the listener is ready...
            Logging::Log("The listener is listening", Logging::Debug);
            sf::TcpSocket *client = new sf::TcpSocket;  //Create a new TCP Socket pointer, client
            if (listener.accept (*client) == sf::Socket::Done) {    //If the listener successfully accepts the client...
                Logging::Log ("Registering a new client: " + client->getRemoteAddress().toString(), Logging::Info);   //Log the connection
                clientList.push_back (client);  //Add the client to the client vector
                socketSelector.add (*client);   //Add the client to the socket selector
            }
            else {      //If the client can't be connected to
                delete client;  //Delete the pointer to the client
            }
        }
        else {      //If the listener isn't ready...
            for (unsigned int i = 0; i < clientList.size(); ++i) {      //For each client in the client list
                sf::TcpSocket* client = clientList.at (i);              //Create a pointer to the client at position i (caching saves on calls)

                if (socketSelector.isReady (*client) ) {                //If the client is sending data
                    if (client->receive (packet) == sf::Socket::Done) { //If the server successfully receives the data
                        std::string subject, details;                   //Declare some variables
                        unsigned char type;
                        packet >> type >> subject >> details;           //Unload the packet into the variables
                        AddInstructionDown (type, subject, details);        //Add a new Network Instruction to the queue
                        Logging::Log(std::to_string(type), Logging::Debug);
                    }
                }
                delete client;      //Delete the client to avoid memory leaks
            }
        }
    }
    else
    {
        Logging::Log("Nothing was ready to receive data", Logging::Debug);
    }
}

void NetworkManager::Heartbeat()
{
    NetworkInstruction networkInstruction(00, "", "");      //Create a new instruction with type 0
    packet.clear();     //Unload the packet
    packet << networkInstruction.type << networkInstruction.subject << networkInstruction.details;      //Load the packet with the new instruction

    for (unsigned int i = 0; i < clientList.size(); ++i) {      //For loop iterates through every client
        clientList.at(i)->send(packet);     //Send them the heartbet packet
    }
    packet.clear();     //Unload the packet again
}

NetworkInstruction &NetworkManager::GetNextInstruction()
{
    NetworkInstruction networkInstruction = downQueue.front();  //Create a Network Instruction using the one at the front of the queue
    downQueue.pop();        //Delete the one at the front of the queue
    return networkInstruction;      //Return the new one
}

void NetworkManager::AddInstructionUp (NetworkInstruction nI)
{
    upQueue.push (nI);      //Add the passed instruction to the queue
}

void NetworkManager::AddInstructionDown (unsigned char t, std::string s, std::string d)
{
    downQueue.push (NetworkInstruction (t, s, d) );       //Add an instruction to the queue using the given parameters
}

void NetworkManager::AddInstructionDown (NetworkInstruction nI)
{
    downQueue.push (nI);      //Add the passed instruction to the queue
}

void NetworkManager::AddInstructionUp (unsigned char t, std::string s, std::string d)
{
    upQueue.push (NetworkInstruction (t, s, d) );       //Add an instruction to the queue using the given parameters
}

bool NetworkManager::DownQueueEmpty()
{
    return downQueue.empty();       //Return whether or not the queue is empty
}
