#ifndef MAP_H
#define MAP_H

struct Tile
{
    enum TileProperty { None = 0, Normal = 0, Spawn = 1, Blocked = 2 };

    TileProperty property = Normal;
    TileProperty temporaryProperty = None;

    unsigned int travelWeight = 1;

    Tile() {}

    Tile (TileProperty tP) {
        property = tP;
        if (tP == Blocked) {
            travelWeight = 100;
        } else {
            travelWeight = 1;
        }
    }
};

class Map
{
    public:
        Map();
        virtual ~Map();

        void AddTile(Tile t);

        unsigned int GetEntityCount();
        bool AddEntity(unsigned int xPos, unsigned int yPos);
        void RemoveEntity(unsigned int xPos, unsigned int yPos);
    protected:

    private:
        unsigned int tileCount = 0;
        unsigned int entityCount = 0;

        Tile tiles[50][50];
};

#endif // MAP_H
