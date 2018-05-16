#include "MapManager.h"

std::vector<Map> MapManager::maps;

unsigned int MapManager::xSize;
unsigned int MapManager::ySize;

bool MapManager::LoadStandardMap()
{
    std::string filepath = "";
    for (unsigned int x = 0; x < 2; x++) {
        for (unsigned int y = 0; y < 2; y++) {
            filepath = "assets/map/tile" + std::to_string(x) + "_" + std::to_string(y) + ".map";
            if (LoadMap(filepath, x, y)) {
                Logging::Log("Loaded map: " + filepath, Logging::Severity::Debug);
            }
        }
    }
    return true;
}

bool MapManager::LoadTestMap()  //To avoid having to create an actual map...
{
    for (unsigned int i = 0; i < 4; i++) {
        Map currentMap;
        for (unsigned int j = 0; j < 250; j++) {
            currentMap.AddTile(Tile(Tile::None));
        }
        maps.push_back(currentMap);
    }
    return true;
}

bool MapManager::LoadMap(std::string filepath, unsigned char worldX, unsigned char worldY)
{
    std::ifstream fileReader;
    fileReader.open(filepath, std::ifstream::in);

    std::string currentLine = "000 n";
    maps.push_back(Map());

    if (!fileReader.good()) {
        return false;
    }

    worldX > xSize ? xSize : xSize = worldX;

    while (fileReader.good()) {
        std::getline(fileReader, currentLine);
        Tile currentTile = ParseLine(currentLine);
        maps.back().AddTile(currentTile);
    }
    return true;
}

Tile MapManager::ParseLine(std::string line)
{
    std::string first, second;

    //Iterate through the line, separating it into two parts
    for (unsigned int i = 0; i < line.size(); i++) {
        if (line.at(i) == ' ') {
            first = line.substr(0, i-1);
            second = line.substr(i+1, line.size() - i);
        }
    }

    std::stringstream sStream;
    sStream << first;
    unsigned char tileID = 0;
    Tile::TileProperty tProperty = Tile::Normal;

    sStream >> tileID;

    if (second == "b") {
        tProperty = Tile::Blocked;
    } else if (second == "s") {
        tProperty = Tile::Spawn;
    }

    return Tile(tProperty);
}

unsigned int MapManager::MapCount()
{
    return maps.size();
}

unsigned int *MapManager::MapSize()
{
    unsigned int mapSize[] = {  };
}

Map *MapManager::GetMap(unsigned int mapIndex)
{
    return &maps.at(mapIndex);
}
