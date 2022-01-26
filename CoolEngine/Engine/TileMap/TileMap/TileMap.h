#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>


#include "Tile.h"
#include "Engine/GameObjects/RenderableGameObject.h"

#include "Engine/Managers/GraphicsManager.h"
#include "Engine/Managers/GameManager.h"

class TileMap  : public RenderableGameObject
{

public:
	// Constructors

	TileMap();

	TileMap(string identifier);

	// Load from file
	TileMap(wstring mapPath, XMFLOAT3 position, XMFLOAT3 scale, string identifier);

	// Load from parameters
	TileMap(int width, int height, string identifier, XMFLOAT3 position);

	//Destructor
	~TileMap();

	void Update(float d);

	Tile* GetTileFromWorldPos(XMFLOAT2 pos);
	Tile* GetTileFromMapPos(int x, int y);

	void SetPassable(int x, int y, bool passable);
	void SetPassable(Tile tile, bool passable);

	void SetTileAtWorldPos(XMFLOAT2 worldPos, Tile* newTile);
	void SetTileAtMapPos(int mapPosX, int mapPosY, Tile* newTile);

	XMFLOAT3 GetScale();
	void SetScale(XMFLOAT3 newScale);

	const int GetWidth() const{ return m_width; }
	const int GetHeight() const { return m_height; }

protected:


private:
	void InitMap();

	void InitMapData(wstring mapPath, XMFLOAT3 position, XMFLOAT3 scale);

	void InitTilePosition(Tile* tile, int row, int column);

	void LoadMap(wstring path);

	void AssignSprites();

	void CreateTile(int row, int column);

	int	m_width;
	int	m_height;
	int	m_totalTiles;

	int	m_tileScaleInt;
	XMFLOAT3 m_tileScale;

	std::vector<std::vector<Tile*>>	m_tiles;

	std::vector<int> m_tileSpriteIndex;
	std::vector<string>	m_spritePaths;
	std::vector<string>	m_animPaths;
};
