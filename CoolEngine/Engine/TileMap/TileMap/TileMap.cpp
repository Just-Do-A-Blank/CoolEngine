#include "TileMap.h"


TileMap::TileMap(string mapPath, XMFLOAT3 position, string identifier)
{
	LoadMap(mapPath);

	GetTransform()->SetPosition(position);

	InitMap();
}

TileMap::TileMap(int width, int height, string identifier, XMFLOAT3 position) : GameObject(identifier)
{
	m_width = width, m_height = height;
	m_totalTiles = m_width * m_height;

	GetTransform()->SetPosition(position);

	InitMap();
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

void TileMap::LoadMap(string path)
{
	string parameter = "<DIMENSIONS>";
	string line;

	string data = "";
	vector<string> dataVec = {};

	ifstream mapFile(path);
	if (mapFile.is_open())
	{
		while (getline(mapFile, line))
		{
			//Size
			if (parameter == "<DIMENSIONS>")
			{
				if (line.find(parameter) != -1)
				{
					for (int i = parameter.size(); (i < line.size() + 1); i++)
					{
						if (line[i] == ',' || line[i] == '*')
						{
							dataVec.push_back(data);

							data.clear();
						}
						else
						{
							if (line[i] != ' ')
								data.push_back(line[i]);
						}
					}

					if (dataVec.size() != 2)
					{
						LOG("ERROR WHEN LOADING TILE MAP DIMENSIONS - INVALID NUMBER OF PARAMETERS")
					}
					else
					{
						m_width  = stoi(dataVec[0]);
						m_height = stoi(dataVec[1]);
					}
				}
				else
				{
					LOG("ERROR WHEN LOADING TILE MAP - PARAMETER 'DIMENSIONS' NOT FOUND");
				}

				parameter = "<SPRITES> ";
			}

			//Sprites
			if (parameter == "<SPRITES>")
			{
				//load vector the same as dimensions
				if (line.find(parameter) != -1)
				{
					for (int i = parameter.size(); (i < line.size() + 1); i++)
					{
						if (line[i] == ',' || line[i] == '*')
						{
						}
					}
				}
				//then load those paths

				parameter = "<ANIMATIONS>";
			}
			
			//Animations
			if (parameter == "<ANIMATIONS>")
			{
				//copy sprites
				if (line.find(parameter) != -1)
				{
					for (int i = parameter.size(); (i < line.size() + 1); i++)
					{
						if (line[i] == ',' || line[i] == '*')
						{
						}
					}
				}
				//KEEP COUNT OF TOTALS
				parameter = "<LAYOUT>";
			}

			//Positions
			if (parameter == "<LAYOUT>")
			{
				if (line.find(parameter) != -1)
				{
					for (int i = parameter.size(); (i < line.size() + 1); i++)
					{
						if (line[i] == ',' || line[i] == '*')
						{
						}
					}
				}
			}

		}

		mapFile.close();
	}
}

//Tile TileMap::GetTileFromWorldPos(int posX, int posY)
//{
//  calculate which tile pos would be
//	use tileMap location and use difference between the two, then divide by tile size
//  if in range, return
//  
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
		LOG("ERROR - INVALID TILEMAP COORDINATE");
		return nullptr;
	}
}

void TileMap::SetPassable(int x, int y, bool passable)
{
	m_tiles[x][y].SetPassable(passable);
}

void TileMap::SetPassable(Tile tile, bool passable)
{
	tile.SetPassable(passable);
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
//void TileMap::SetTileAtMapPos(int posX, int posY, Tile* tile)
//{
//}
