#include "TileMap.h"


TileMap::TileMap(int width, int height)
{
	m_width = width, m_height = height;
	InitMap();
}

TileMap::~TileMap()
{
}

void TileMap::InitMap()
{
	m_tiles.reserve(m_width * m_height);

	int test = 0;
	for (int i = 0; i < m_height; ++i)
	{
		for (int j = 0; i < m_width; ++j)
		{
			m_tiles[m_height][m_width] = Tile(test);
			test++;
		}
	}
}

void TileMap::PopulateMap()
{
}

Tile TileMap::GetTileFromWorldPos(int posX, int posY)
{
	return Tile(1);
}

Tile TileMap::GetTileFromMapPos(int x, int y)
{
	return m_tiles[y][x];
}

void TileMap::SetTileAtWorldPos(int posX, int posY)
{
}

void TileMap::SetTileAtMapPos(int posX, int posY)
{
}
