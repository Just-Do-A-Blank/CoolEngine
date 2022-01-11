#pragma once

#include <vector>
#include <iostream>

#include "Tile.h"

class TileMap
{
	//width, height, tiles, sprites,

public:
	void testFunc() { std::cout << "Test" << std::endl; }

	TileMap(int width, int height);
	~TileMap();

	Tile					GetTileFromWorldPos(int posX, int posY);
	Tile*					GetTileFromMapPos(int x, int y);

protected:
	

private:
	void					InitMap();
	//void					PopulateMap(); // TODO ONCE LEVEL MAPS ARE DESIGNED
	//void					LoadSprites(); // TODO ONCE LOADING IS IMPLEMENTED
	void					InitEdges();


	/// <summary>Allows the user to define which edges of a tile are collideable. x, y = TileMap Coordinates of tile to be modified . N, E, S & W - True = Collideable Edge</summary>
	void					SetEdges(int x, int y, bool N, bool E, bool S, bool W); // TODO - Fix summary
	

	//void					SetTileAtWorldPos(int posX, int posY);
	//void					SetTileAtMapPos(int posX, int posY);


	int									m_width;
	int									m_height;

	
	std::vector<std::vector<Tile>>		m_tiles;


	std::vector<int>					m_sprites;
};