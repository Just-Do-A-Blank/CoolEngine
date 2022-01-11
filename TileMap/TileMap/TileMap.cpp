#include "TileMap.h"


TileMap::TileMap(int width, int height)
{
	m_width = width, m_height = height;
	InitMap();
	InitEdges();
}

TileMap::~TileMap()
{
}

void TileMap::InitMap()
{
	m_tiles.resize(m_height);

	int test = 0;
	for (int i = 0; i < m_height; ++i)
	{
		for (int j = 0; j < m_width; ++j)
		{
			if (j == 0)
			{
				m_tiles[i].resize(m_width);
			}
			m_tiles[i][j] = Tile(test);
			test++;
		}
	}
}

void TileMap::InitEdges() // I don't know how this works but it does
{
	char edge = 'N';
	for (int j = 0; j < 10; j++)
	{
		switch (edge)
		{
			case('N'):
			{
				m_tiles[0][j].SetEdgeN(true);

				if (j == 9)
				{
					edge = 'S';
				}
			}
			case('S'):
			{
				m_tiles[m_height - 1][j].SetEdgeS(true);

				if (j == 9)
				{
					edge = 'W';
				}
			}
			case('W'):
			{
				m_tiles[j][0].SetEdgeW(true);

				if (j == 9)
				{
					edge = 'E';
				}
			}
			case('E'):
			{
				m_tiles[j][m_width - 1].SetEdgeE(true);

				if (j == 9)
				{
					edge = ' ';
				}
			}
		}
	}
}

Tile TileMap::GetTileFromWorldPos(int posX, int posY)
{
	return Tile();
}

Tile* TileMap::GetTileFromMapPos(int x, int y)
{
	if (x < m_width && x >= 0 && y < m_height && y >= 0)
	{
		return &m_tiles[x][y];
	}
	else
	{
		std::cout << ("ERROR - INVALID TILEMAP COORDINATE: x:") << x << " y:" << y << "\n";
		std::cout << ("Bounds for TileMap are ") << m_width << " width and " << m_height << " height\n";
		return nullptr;
	}
}

void TileMap::SetEdges(int x, int y, bool N, bool S, bool W, bool E)
{
	Tile* pTile = GetTileFromMapPos(x, y);

	if (pTile->GetID() != -25)
	{
		pTile->SetEdges(N, S, W, E);
	}
}

//void TileMap::SetTileAtWorldPos(int posX, int posY)
//{
//}
//
//void TileMap::SetTileAtMapPos(int posX, int posY)
//{
//}
