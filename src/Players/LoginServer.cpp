#include "LoginServer.h"

bool LoginServer::AttemptLogin(std::string userHash, std::string passHash)
{
    std::string query = "SELECT Accounts.* FROM Accounts WHERE Accounts.Email==\"";
    query += userHash + "\";";

    Logging::Log("The query sent was: " + query, Logging::Severity::Debug);

    QueryResult qResult = Database::Execute(query);

    Logging::Log("The pass field contains: " + qResult.QueryColumn(("Pass")), Logging::Severity::Debug);

    if (qResult.QueryColumn("Pass") == passHash) {
        query = "UPDATE Accounts SET Accounts.LoggedIn=1 WHERE Accounts.Email==\"" + userHash  + "\";";
        Logging::Log("The client successfully logged in", Logging::Severity::Debug);
        Database::Execute(query);
        return true;
    }

    return false;
}

void LoginServer::Logout(std::string userHash)
{
    std::string query = "UPDATE Accounts SET Accounts.LoggedIn=0 WHERE Accounts.Email==\"" + userHash + "\";";
    Database::Execute(query);
}
