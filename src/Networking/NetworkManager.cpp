#include "NetworkManager.h"

sf::TcpListener NetworkManager::listener;                   //Forward declaration as these are all static members...
sf::SocketSelector NetworkManager::socketSelector;

sf::Packet NetworkManager::packet;

std::vector<sf::TcpSocket*> NetworkManager::clientList;
std::queue<std::pair<NetworkInstruction*, sf::TcpSocket*>> NetworkManager::downQueue;
std::queue<std::pair<NetworkInstruction*, sf::TcpSocket*>> NetworkManager::upQueue;

void NetworkManager::Parse(std::pair<NetworkInstruction*, sf::TcpSocket*> &instruction) {
    Logging::Log("Parsing instruction: " + std::to_string(instruction.first->type) + ", " + instruction.first->subject + ", " + instruction.first->details, Logging::Severity::Debug);
    switch (instruction.first->type) {
        case 0: //Disconnect
            {
                Logging::Log("Disconnect received", Logging::Severity::Debug);
                Logging::Log("The socket gave an IP address of: " + instruction.second->getRemoteAddress().toString(), Logging::Severity::Debug);
                if (instruction.first->details == "false") {
                    Logging::Log ("Client disconnected from: " + instruction.second->getRemoteAddress().toString(), Logging::Severity::Debug);
                    Logging::Log ("The client gave the username: " + std::string(instruction.first->subject), Logging::Severity::Debug);
                    if (instruction.first->subject != "") {
                        LoginServer::Logout(instruction.first->subject);
                    }
                    Logging::Log("Removing the client socket from: " + instruction.second->getRemoteAddress().toString(), Logging::Severity::Debug);
                    socketSelector.remove(*instruction.second);
                    AddInstructionUp(00, instruction.first->subject, "true", instruction.second);
                }
                break;
            }
        case 1: //Authentication
            {
                std::string username = instruction.first->subject;
                std::string passHash = instruction.first->details;
                sf::TcpSocket *s = instruction.second;
                if (instruction.second) {
                    Logging::Log("Client is trying to authenticate with " + username + ", " + passHash, Logging::Severity::Debug);
                    if (LoginServer::AttemptLogin(username, passHash)) {
                        AddInstructionUp(02, username, "true", s);
                        instruction.first->Clear();
                        break;
                    } else {
                        Logging::Log("The client provided incorrect credentials", Logging::Severity::Debug);
                        AddInstructionUp(02, username, "false", s);
                        Logging::Log("The client was informed", Logging::Severity::Debug);
                        break;
                    }
                }
                break;

            }
    }

    delete instruction.first;
}

bool NetworkManager::Initialise()
{
    if (listener.listen(6401) != sf::Socket::Done) {                //If the listener fails to start listening on 6401...
        Logging::Log("Listener failed to listen on port " + std::to_string(listener.getLocalPort()), Logging::Severity::Important);     //Error message
        return false;
    } else {                                                        //Otherwise...
        Logging::Log("Listener is now listening on port " + std::to_string(listener.getLocalPort()), Logging::Severity::Important);     //Success message
    }

    socketSelector.add (listener);      //Add the TCP Listener to the Socket Selector

    Logging::Log("The remote address of the server is " + sf::IpAddress::getPublicAddress().toString(), Logging::Severity::Info);
    Logging::Log("The local address of the server is " + sf::IpAddress::getLocalAddress().toString(), Logging::Severity::Info);
    return true;
}

void NetworkManager::Update()
{

    packet.clear();

    if (socketSelector.wait(sf::seconds(0.1f))) {           //Wait until a socket is ready to receive, with a timeout of 0.1 seconds
        if (socketSelector.isReady (listener) ) {       //If the listener is ready...
            Logging::Log("The listener is listening", Logging::Severity::Debug);
            sf::TcpSocket *client = new sf::TcpSocket;  //Create a new TCP Socket pointer, client
            if (listener.accept (*client) == sf::Socket::Done) {    //If the listener successfully accepts the client...
                Logging::Log ("Registering a new client: " + client->getRemoteAddress().toString(), Logging::Severity::Info);   //Log the connection
                clientList.push_back (client);  //Add the client to the client vector
                client->setBlocking(true);
                socketSelector.add (*client);   //Add the client to the socket selector
                Logging::Log("The client list now contains " + std::to_string(clientList.size()) + " items", Logging::Severity::Debug);
            }
            else {      //If the client can't be connected to
                delete client;  //Delete the pointer to the client
            }
        }
        else {      //If the listener isn't ready...
            for (std::vector<sf::TcpSocket*>::iterator it = clientList.begin(); it != clientList.end(); ++it) {      //For each client in the client list
                sf::TcpSocket* client = *it;              //Create a pointer to the client at position i (caching saves on calls)

                Logging::Log("A client is sending data" , Logging::Severity::Debug);

                if (socketSelector.isReady (*client) ) {                //If the client is sending data
                    NetworkInstruction *instruction = new NetworkInstruction();
                    instruction->Receive(*client);
                    AddInstructionDown(instruction, client);
                }
            }
        }
    }
    else
    {

    }

    while (!DownQueueEmpty()) {
        std::pair<NetworkInstruction*, sf::TcpSocket*> instruction = downQueue.front();
        downQueue.pop();
        Parse(instruction);
    }

    while (!UpQueueEmpty()) {
        std::pair<NetworkInstruction*, sf::TcpSocket*> instruction = upQueue.front();
        upQueue.pop();
        if (instruction.second) {       //If there actually is an open socket...
            instruction.first->Send(*(instruction.second));
            Logging::Log("Sent " + std::to_string(instruction.first->type) + ", " + instruction.first->subject.toAnsiString() + ", " + instruction.first->details.toAnsiString(), Logging::Severity::Debug);
            delete instruction.first;
        }
    }

    RemoveClients();        //Do some cleanup of disconnected clients

}

void NetworkManager::Heartbeat()            //Function not used because it's basically useless with such a low client count
{
    NetworkInstruction networkInstruction(00, "", "");      //Create a new instruction with type 0
    packet.clear();     //Unload the packet
    packet << networkInstruction.type << networkInstruction.subject << networkInstruction.details;      //Load the packet with the new instruction

    for (unsigned int i = 0; i < clientList.size(); ++i) {      //For loop iterates through every client
        clientList.at(i)->send(packet);     //Send them the heartbet packet
    }
    packet.clear();     //Unload the packet again
}

void NetworkManager::RemoveClients() {
    for (std::vector<sf::TcpSocket*>::iterator it = clientList.begin(); it != clientList.end(); ++it) {
        {
            if ((*it) == nullptr) {
                clientList.erase(it);
                Logging::Log("The client list now contains " + std::to_string(clientList.size()) + " items", Logging::Severity::Debug);
            }
        }
    }
}

std::pair<NetworkInstruction*, sf::TcpSocket*> &NetworkManager::GetNextInstruction()
{
    std::pair<NetworkInstruction*, sf::TcpSocket*> networkInstruction = downQueue.front();  //Create a Network Instruction using the one at the front of the queue
    downQueue.pop();                //Delete the one at the front of the queue
    return networkInstruction;      //Return the new one
}

void NetworkManager::AddInstructionUp (NetworkInstruction *nI, sf::TcpSocket *client)
{
    upQueue.push (std::make_pair(nI, client));      //Add the passed instruction to the queue
}

void NetworkManager::AddInstructionDown (unsigned char t, std::string s, std::string d, sf::TcpSocket *client)
{
    downQueue.push (std::make_pair(new NetworkInstruction (t, s, d), client));       //Add an instruction to the queue using the given parameters
}

void NetworkManager::AddInstructionDown (NetworkInstruction *nI, sf::TcpSocket *client)
{
    downQueue.push (std::make_pair(nI, client));      //Add the passed instruction to the queue
}

void NetworkManager::AddInstructionUp (unsigned char t, std::string s, std::string d, sf::TcpSocket *client)
{
    upQueue.push (std::make_pair(new NetworkInstruction (t, s, d), client));       //Add an instruction to the queue using the given parameters
}

bool NetworkManager::UpQueueEmpty()     //Return whether or not the queue is empty
{
    return upQueue.empty();
}

bool NetworkManager::DownQueueEmpty()
{
    return downQueue.empty();       //Return whether or not the queue is empty
}
