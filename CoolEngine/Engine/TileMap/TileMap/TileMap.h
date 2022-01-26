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
	TileMap(wstring mapPath, XMFLOAT3 position, float tileDimensions, string identifier);

	// Load from parameters
	TileMap(int width, int height, string identifier, XMFLOAT3 position, float tileDimensions);

	//Destructor
	~TileMap();

	void Update(float d);

	bool GetTileFromWorldPos(XMFLOAT2 pos, Tile*& ptile, int* prow = nullptr, int* pcolumn = nullptr);
	bool GetTileFromMapPos(int x, int y, Tile*& ptile);

	void SetPassable(int x, int y, bool passable);

	void SetTileAtWorldPos(XMFLOAT2 worldPos, Tile* newTile);
	void SetTileAtMapPos(int mapPosX, int mapPosY, Tile* newTile);

	XMFLOAT3 GetTileScale();
	void SetTileScale(XMFLOAT3 newScale);

	const int GetWidth() const{ return m_width; }
	const int GetHeight() const { return m_height; }

	bool CreateTile(int row, int column, Tile*& ptile);

protected:


private:
	void InitMap();

	void InitTilePosition(Tile* tile, int row, int column);

	bool Load(wstring path);
	bool Save(wstring path);

	bool GetCoordsFromWorldPos(int* prow, int* pcolumn, const XMFLOAT2& pos);

	int	m_width;
	int	m_height;
	int	m_totalTiles;

	int	m_tileScaleInt;
	XMFLOAT3 m_tileScale;

	std::vector<std::vector<Tile*>>	m_tiles;

	std::vector<wstring> m_spritePaths;
	std::vector<wstring> m_animPaths;
};
