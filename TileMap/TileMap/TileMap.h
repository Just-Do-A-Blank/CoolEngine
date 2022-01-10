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
	Tile					GetTileFromMapPos(int x, int y);

protected:
	

private:
	void					InitMap();
	void					PopulateMap();
	//void					LoadSprites(); // TODO ONCE LOADING IS IMPLEMENTED

	

	void					SetTileAtWorldPos(int posX, int posY);
	void					SetTileAtMapPos(int posX, int posY);


	int									m_width;
	int									m_height;
	std::vector<std::vector<Tile>>		m_tiles;
	std::vector<int>					m_sprites;
};