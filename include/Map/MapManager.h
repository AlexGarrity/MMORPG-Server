#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include "Map.h"
#include "Logging.h"

class MapManager
{
    public:
        static bool LoadTestMap();
        static bool LoadStandardMap();
        static bool LoadMap(std::string filepath, unsigned char worldX, unsigned char worldY);
        static Tile ParseLine(std::string line);

        static unsigned int MapCount();
        static unsigned int *MapSize();
        static Map *GetMap(unsigned int mapIndex);

    protected:

    private:
        static unsigned int xSize;
        static unsigned int ySize;

        static std::vector<Map> maps;
};

#endif // MAPMANAGER_H
