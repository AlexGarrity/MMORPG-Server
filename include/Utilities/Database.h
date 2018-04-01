#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <map>

#include <sqlite3.h>

#include "Logging.h"

//Only capable of handling a single result, but only a single should be required
struct QueryResult {

    std::map<std::string, std::string> columns;

    void AddColumn(std::string cName, std::string val) {
        columns.insert(std::make_pair(cName, val));
    }

    std::string QueryColumn(std::string cName) {
        std::map<std::string, std::string>::iterator it;
        for (it = columns.begin(); it != columns.end(); ++it) {
            if (it->first == cName) {
                return it->second;
            }
        }
        return "NULL";
    }

    void OutputResult() {
        std::map<std::string, std::string>::iterator it;
        for (it = columns.begin(); it != columns.end(); ++it) {
            Logging::Log(it->first + "\t:\t" + it->second, Logging::Debug);
        }
    }
};

class Database
{
    public:
        static bool Initialise(std::string dbName);
        static void Close();

        static QueryResult Execute(std::string query);

    protected:

    private:

        static bool TestDatabase();

        static int callback(void *NotUsed, int argc, char **argv, char **azColName);

        static sqlite3 *database;
        static std::string databaseName;
        static QueryResult currentResult;
};

#endif // DATABASE_H
