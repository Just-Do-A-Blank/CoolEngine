#include "TileMap.h"

int main()
{
	TileMap TestMap = TileMap(10, 10);

	std::cout << "Found Tile " << TestMap.GetTileFromMapPos(0, 1)->GetID();
}