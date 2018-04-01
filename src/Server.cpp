#include "Server.h"

float Server::elapsedTime = 0;
unsigned char Server::tick = 0;
unsigned char Server::lastTick = 0;

bool Server::Start()
{
    NetworkManager::Initialise();

    if (!Database::Initialise("database")) {
        return false;
    }

    while (true) {
        Update();
    }
}

void Server::Update()
{
    lastTick = tick;
    elapsedTime += Timing::deltaTime;

    if (elapsedTime > 0.5f) {
        elapsedTime = 0;
        tick += 1;

        if (tick != lastTick) {
            if (tick % 1 == 0) {
                FastUpdate();
            }

            if (tick % 2 == 0) {
                MediumUpdate();
            }

            if (tick % 8 == 0) {
                LongUpdate();
                tick = 0;
            }
        }
    }

    Timing::Update();
}

void Server::ObjectUpdate() {
    ObjectManager::Update();
}

void Server::NetworkUpdate() {
    NetworkManager::Update();
    InstructionParser::Parse();
}

void Server::FastUpdate()
{
    Logging::Log("Fast Update", Logging::Debug);
    ObjectUpdate();
}

void Server::MediumUpdate()
{
    Logging::Log("Medium Update", Logging::Debug);
    NetworkUpdate();
}

void Server::LongUpdate()
{
    //Logging::Log("Long Update");
    //NetworkManager::Heartbeat();  Unused because my max player count isn't going anywhere above 50 at any given point
}
