#include "TileMap.h"

int main()
{
	TileMap TestMap = TileMap(10, 10);

	TestMap.GetTileFromMapPos(60, 8).getID();
}