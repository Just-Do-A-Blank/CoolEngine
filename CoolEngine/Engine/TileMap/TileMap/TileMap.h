#pragma once

#define TILE_SIZE 1

#include <vector>
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>


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

	void					Update(float d);
	void					Render(ID3D11DeviceContext* pcontext, ConstantBuffer<PerInstanceCB>* pconstantBuffer);

	Tile					GetTileFromWorldPos(int posX, int posY);
	Tile*					GetTileFromMapPos(int x, int y);

protected:
	

private:
	void					InitMap();
	void					InitTilePosition(Tile tile, int row, int column);

	void					LoadMap(string path);

	void					AssignSprites();

	void					SetPassable(int x, int y, bool passable);
	void					SetPassable(Tile tile, bool passable);

	void					SetTileAtWorldPos(int posX, int posY, Tile newTile);
	void					SetTileAtMapPos(int mapPosX, int mapPosY, Tile newTile);


	int									m_width;
	int									m_height;
	int									m_totalTiles;


	
	std::vector<std::vector<Tile>>		m_tiles;
	
	std::vector<int>					m_tileSpriteIndex;
	std::vector<string>					m_spritePaths;
	std::vector<string>					m_animPaths;
};