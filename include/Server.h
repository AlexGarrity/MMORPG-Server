#ifndef SERVER_H
#define SERVER_H

#include <thread>

#include "Timing.h"
#include "Logging.h"

#include "NetworkManager.h"
#include "ObjectManager.h"
#include "PlayerManager.h"
#include "Database.h"

class Server
{
public:
    static bool Start();

protected:

private:
    static void Update();

    static void NetworkUpdate();
    static void ObjectUpdate();

    static void FastUpdate();
    static void MediumUpdate();
    static void LongUpdate();

    static float elapsedTime;
    static unsigned char tick;
    static unsigned char lastTick;
};

#endif // SERVER_H
