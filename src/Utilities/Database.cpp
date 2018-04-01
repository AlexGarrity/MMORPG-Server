#include "Database.h"

sqlite3 *Database::database;            //Forward declaration of statics
std::string Database::databaseName;
QueryResult Database::currentResult;

//Initialises the database and runs a test query
bool Database::Initialise(std::string dbName)
{
    int errorCode;
    databaseName = dbName;
    errorCode = sqlite3_open("database.db", &database);     //Open the database, and return its error code into errorCode

    Logging::Log("Error code when opening database: " + std::to_string(errorCode), Logging::Debug);        //Log the error code (should be 0)

    if (errorCode != 0) {       //If the error code isn't 0...
        Logging::Log("Could not connect to the database: " + databaseName, Logging::Important);     //Log the error
        sqlite3_close(database);        //Close the database
        return false;       //Return false
    }
    if (!TestDatabase()) {
        return false;       //If the test fails, return false
    }
    Logging::Log("Connected to the database", Logging::Debug);      //Log that the server has successfully connected
    return true;        //Return true
}

void Database::Close()
{
    //Close the database.  Called when the server closes.
    sqlite3_close(database);
}

QueryResult Database::Execute(std::string query)
{
    /*
    Removed because performing the operation in three separate steps is much more useful

    char *errorMessage = nullptr;
    sqlite3_exec(database, query.c_str(), callback, 0, &errorMessage);
    return currentResult;
    */

    //Create a new statement, initialised to a null pointer
    sqlite3_stmt *statement = nullptr;

    //Prepare the statement by inputting the query.  And don't case the query to a c_string for the size, it cuts off the semicolon.
    sqlite3_prepare_v2(database, query.c_str(), sizeof(query), &statement, nullptr);

    QueryResult qResult;            //Create a QueryResult structure
    unsigned int columnCount = 0;   //Forward declare some variables for efficiency
    unsigned int stepValue = 0;
    std::string columnName;
    std::string columnValue;

    //While the statement still has data left in it, get data from it (step through it)...

        stepValue = sqlite3_step(statement);

        columnCount = sqlite3_column_count(statement);      //Get the column count from the statment

        Logging::Log("Column count is: " + std::to_string(columnCount), Logging::Debug);

        //Iterate through the columns, using variable i
        for (unsigned int i = 0; i < columnCount; i++) {
            columnValue = reinterpret_cast<const char*>(sqlite3_column_text(statement, i)); //Get the value at index i
            columnName =  reinterpret_cast<const char*>(sqlite3_column_name(statement, i));//Get the column name at index i
            qResult.AddColumn(columnName, columnValue);
        }

    qResult.OutputResult();

    //Runs a destructor for the statement
    sqlite3_finalize(statement);
    //Return the result of the query as a struct
    return qResult;
}

bool Database::TestDatabase()
{
    //Execute the following command to check the database size (should have at least one dummy entry)
    QueryResult result = Execute("SELECT Accounts.* FROM Accounts;");
    //Log the number of entries
    Logging::Log("Database returned " + std::to_string(result.columns.size()) + " items", Logging::Debug);
    if (result.QueryColumn("Email") != "" && result.QueryColumn("Email") != "NULL") {
        //Return true as the table is proven to have been populated
        return true;
    }
    //Return false because the table is empty (there should at least be one dummy value)
    return false;
}

//Callback that I borrowed from the reference.  Not used because it only prints the values.
//Too irritating (or, rather, specific) to be worth using.
int Database::callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}
