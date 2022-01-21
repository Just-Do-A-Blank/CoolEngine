#include "TileMap.h"


TileMap::TileMap(int width, int height, string identifier, XMFLOAT3 position) : GameObject(identifier)
{
	m_width = width;
	m_height = height;

	GetTransform()->SetPosition(position);

	InitMap();
	InitEdges();
}

TileMap::~TileMap()
{
	m_tiles.clear(); // Run first to allow each tile to release its memory, but this does not release the memory for the vector
	// Memory reserved by vectors can be deleted by swapping a vector with an uninitialized vector, which means that all memory allocated to the initial vector is released. The temporary vector is then destroyed, since it is temporary
	vector<vector<Tile>> tempVector;
	m_tiles.swap(tempVector); 
}

void TileMap::InitMap()
{
	m_tiles.resize(m_height);

	int ID = 0;
	for (int i = 0; i < m_height; ++i)
	{
		for (int j = 0; j < m_width; ++j)
		{
			if (j == 0)
			{
				m_tiles[i].resize(m_width);
			}
			m_tiles[i][j] = Tile(ID, "test");
			InitTilePosition(m_tiles[i][j], i, j);
			++ID;
		}
	}
}

void TileMap::LoadSprites()
{
	
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

void TileMap::InitTilePosition(Tile tile, int row, int column)
{
	XMFLOAT3 position = GetTransform()->GetPosition();

	int spriteSize = 1;

	float Xoffset = 0;
	float Yoffset = 0;

	switch (m_width % 2)
	{
		case(0):
		{
			Xoffset = ((m_width - 1) * 0.5);
			position.x = (position.x + ((column - Xoffset) * spriteSize));

			break;
		}

		case(1):
		{
			Xoffset = m_width / 2;
			position.x = (position.x + ((column - Xoffset) * spriteSize));

			break;
		}

		default:
		{
			break;
		}
	}

	switch (m_height % 2)
	{
		case(0):
		{
			Yoffset = ((m_height - 1) * 0.5);
			position.y = (position.y + ((row - Yoffset) * spriteSize));

			break;
		}

		case(1):
		{
			Yoffset = m_height / 2;
			position.y = (position.y + ((row - Yoffset) * spriteSize));

			break;
		}

		default:
		{
			break;
		}
	}


	tile.GetTransform()->SetPosition(position);
}

//Tile TileMap::GetTileFromWorldPos(int posX, int posY)
//{
//	return Tile;
//}

Tile* TileMap::GetTileFromMapPos(int x, int y)
{
	if (x < m_width && x >= 0 && y < m_height && y >= 0)
	{
		return &m_tiles[x][y];
	}
	else
	{
		//LOG("ERROR - INVALID TILEMAP COORDINATE: x:") << x << " y:" << y << "\n";
		//LOG("Bounds for TileMap are " + string(m_width) + " width and " + string(m_height) + " height\n");
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

void TileMap::Update(float d)
{
	for (int i = 0; i < m_totalTiles; i++)
	{
		//TileGameObject tile = m_tiles[i];
	}
}

//void TileMap::SetTileAtWorldPos(int posX, int posY)
//{
//}
//
//void TileMap::SetTileAtMapPos(int posX, int posY)
//{
//}
