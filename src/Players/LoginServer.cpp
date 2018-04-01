#include "LoginServer.h"

bool LoginServer::AttemptLogin(std::string userHash, std::string passHash)
{
    std::string query = "SELECT Accounts.Pass, Accounts.LoggedIn FROM Accounts WHERE Accounts.Email=\"";
    query += userHash + "\";";

    QueryResult qResult = Database::Execute(query);
    if (qResult.QueryColumn("Pass") == passHash) {
        return true;
        query = "UPDATE Accounts SET Accounts.LoggedIn=1 WHERE Accounts.Email=\"" + userHash  + "\";";
        Database::Execute(query);
    }

    return false;
}

void LoginServer::Logout(std::string userHash)
{
    std::string query = "UPDATE Accounts SET Accounts.LoggedIn=0 WHERE Accounts.Email=\"" + userHash + "\";";
    Database::Execute(query);
}
