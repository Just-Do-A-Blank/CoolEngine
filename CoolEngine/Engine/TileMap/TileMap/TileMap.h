#pragma once

#include <vector>
#include <iostream>

#include "TileGameObject.h"
#include "Engine/Managers/GraphicsManager.h"

class TileMap
{

public:
	void testFunc() { std::cout << "Test" << std::endl; }

	TileMap(int width, int height);
	~TileMap();

	//TileGameObject					GetTileFromWorldPos(int posX, int posY);
	TileGameObject*					GetTileFromMapPos(int x, int y);

protected:
	

private:
	void					InitMap();
	//void					PopulateMap(); // TODO ONCE LEVEL MAPS ARE DESIGNED
	void					LoadSprites(); // TODO ONCE LOADING IS IMPLEMENTED
	void					InitEdges();


	//Allows the user to define which edges of a tile are collideable. x, y = TileMap Coordinates of tile to be modified . N, E, S & W - True = Collideable Edge
	void					SetEdges(int x, int y, bool N, bool E, bool S, bool W); // TODO - Fix summary
	
	void					Update(float d);

	//void					SetTileAtWorldPos(int posX, int posY);
	//void					SetTileAtMapPos(int posX, int posY);


	int									m_width;
	int									m_height;
	int									m_totalTiles;
	
	std::vector<std::vector<TileGameObject>>		m_tiles;
};