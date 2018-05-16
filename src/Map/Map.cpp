#include "Map.h"

Map::Map()
{
    //ctor
}

Map::~Map()
{

}

void Map::AddTile(Tile t)
{
    unsigned int xPos = tileCount % 50;
    unsigned int yPos = tileCount / 50;

    tiles[xPos][yPos] = t;
    tileCount += 1;
}

unsigned int Map::GetEntityCount()
{
    return entityCount;
}

bool Map::AddEntity(unsigned int xPos, unsigned int yPos)
{
   /* if (tiles[xPos][yPos].tileType != Tile::Spawn) {
        return false;
    }
    */
    if (tiles[xPos][yPos].temporaryProperty == Tile::Blocked) {
        return false;
    }

    tiles[xPos][yPos].temporaryProperty = Tile::Blocked;
    entityCount ++;
    return true;
}

void Map::RemoveEntity(unsigned int xPos, unsigned int yPos)
{
    tiles[xPos][yPos].temporaryProperty = Tile::None;
}
