#include "Server.h"

float Server::elapsedTime = 0;
unsigned char Server::tick = 0;
unsigned char Server::lastTick = 0;

bool Server::Start()
{
    if (!NetworkManager::Initialise()) {
        return false;
    }
    if (!Database::Initialise("database")) {
        return false;
    }
    if (!MapManager::LoadTestMap()) {
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

    if ((int) elapsedTime % 1 == 0) {
        FastUpdate();
    }
    if ((int) elapsedTime % 4 == 0) {
        MediumUpdate();
    }
    if ((int) elapsedTime % 8 == 0) {
        LongUpdate();
    }

    Timing::Update();
}

void Server::ObjectUpdate() {
    ObjectManager::Update();
}

void Server::NetworkUpdate() {
    NetworkManager::Update();
}

void Server::FastUpdate()
{
    ObjectUpdate();
}

void Server::MediumUpdate()
{
    NetworkUpdate();
}

void Server::LongUpdate()
{
    //Logging::Log("Long Update");
    //NetworkManager::Heartbeat();  Unused because my max player count isn't going anywhere above 5 at any given point
}
