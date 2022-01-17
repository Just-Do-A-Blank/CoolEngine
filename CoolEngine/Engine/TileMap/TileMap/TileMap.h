#pragma once

#include <vector>
#include <iostream>
#include <fstream>

#include "Tile.h"
#include "Engine/GameObjects/GameObject.h"
#include "Engine/Managers/GraphicsManager.h"

class TileMap  : public GameObject
{

public:
	// Constructors

	// Load from file
	TileMap(string mapPath, XMFLOAT3 position, string identifier);

	// Load from parameters
	TileMap(int width, int height, string identifier, XMFLOAT3 position);

	//Destructor
	~TileMap();

	//Tile					GetTileFromWorldPos(int posX, int posY);
	Tile*					GetTileFromMapPos(int x, int y);

protected:
	

private:
	void					InitMap();
	void					InitTilePosition(Tile tile, int row, int column);

	void					LoadMap(string path);
	
	void					Update(float d);

	void					SetPassable(int x, int y, bool passable);
	void					SetPassable(Tile tile, bool passable);

	//void					SetTileAtWorldPos(int posX, int posY);
	//void					SetTileAtMapPos(int posX, int posY);


	int									m_width;
	int									m_height;
	int									m_totalTiles;
	
	std::vector<std::vector<Tile>>		m_tiles;
};