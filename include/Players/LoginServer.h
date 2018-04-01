#ifndef LOGINSERVER_H
#define LOGINSERVER_H

#include <algorithm>

#include "Database.h"

class LoginServer
{
    public:
        static bool AttemptLogin(std::string userHash, std::string passHash);
        static void Logout(std::string userHash);

    protected:

    private:
};

#endif // LOGINSERVER_H
