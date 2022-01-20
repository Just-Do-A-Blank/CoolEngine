#pragma once

#include <vector>
#include <iostream>

#include "Tile.h"
#include "Engine/GameObjects/GameObject.h"
#include "Engine/Managers/GraphicsManager.h"

class TileMap  : public GameObject
{

public:
	void testFunc() { std::cout << "Test" << std::endl; }

	TileMap();

	TileMap(int width, int height, string identifier, XMFLOAT3 position);
	~TileMap();

	//Tile					GetTileFromWorldPos(int posX, int posY);
	Tile*					GetTileFromMapPos(int x, int y);

protected:
	

private:
	void					InitMap();
	void					InitEdges();
	void					InitTilePosition(Tile tile, int row, int column);
	//void					PopulateMap(); // TODO ONCE LEVEL MAPS ARE DESIGNED

	//						Loads sprites for the current room 
	void					LoadSprites(); //Loading sprites through the Tile Map when it is initialized prevents all tiles doing it themselves, saving many pointless load calls. Once all sprites are loaded, AssignSprites will be run to pass the sprite to the tiles that need it
	//void					AssignSprites(); // TODO ONCE MAP DATA FORMATTING IS DEFINED


	//Allows the user to define which edges of a tile are collideable. x, y = TileMap Coordinates of tile to be modified.   N, E, S & W - True = Collideable Edge
	void					SetEdges(int x, int y, bool N, bool E, bool S, bool W);
	
	void					Update(float d);

	//void					SetTileAtWorldPos(int posX, int posY);
	//void					SetTileAtMapPos(int posX, int posY);


	int									m_width;
	int									m_height;
	int									m_totalTiles;
	
	std::vector<std::vector<Tile>>		m_tiles;
};