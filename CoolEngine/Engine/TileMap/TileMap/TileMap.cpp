#include "TileMap.h"

TileMap::TileMap() : RenderableGameObject()
{
}

TileMap::TileMap(string identifier) : RenderableGameObject(identifier)
{
}

TileMap::TileMap(wstring mapPath, XMFLOAT3 position, XMFLOAT3 scale, string identifier) : RenderableGameObject(identifier)
{
	LoadMap(mapPath);

	GetTransform()->SetPosition(position);
	GetTransform()->SetScale(scale);
	m_tileScaleInt = scale.x;

	InitMap();

	GetTileFromWorldPos(XMFLOAT2(-50, -50));

	AssignSprites();
}

TileMap::TileMap(int width, int height, string identifier, XMFLOAT3 position) : RenderableGameObject(identifier)
{
	m_width = width;
	m_height = height;

	m_totalTiles = m_width * m_height;

	m_transform->SetScale(XMFLOAT3(25, 25, 1));
	m_transform->SetPosition(position);

	InitMap();

	GetTileFromWorldPos(XMFLOAT2(-25, -25));
}

TileMap::~TileMap()
{
	m_tiles.clear(); // Run first to allow each tile to release its memory, but this does not release the memory for the vector
	
	// Memory reserved by vectors can be deleted by swapping a vector with an uninitialized vector, which means that all memory allocated to the initial vector is released. The temporary vector is then destroyed, since it is temporary
	vector<vector<Tile*>> tileEmptyVector;
	vector<int> intEmptyVector;
	vector<string>	stringEmptyVector;

	m_tiles.swap(tileEmptyVector);
	m_tileSpriteIndex.swap(intEmptyVector);
	m_spritePaths.swap(stringEmptyVector);
	m_animPaths.swap(stringEmptyVector);
}

void TileMap::Update(float d)
{
	for (int i = 0; i < m_height; i++)
	{
		for (int j = 0; j < m_width; j++)
		{
			m_tiles[i][j]->Update();
		}
	}
}

void TileMap::InitMap() // Create and store tiles in m_Tiles
{
	GameManager* pGameManager = GameManager::GetInstance();

	XMFLOAT3 scale = GetTransform()->GetScale();

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

			string name = "Tile-" + to_string(ID);

			m_tiles[i][j] = nullptr;
		}
	}
}

void TileMap::InitMapData(wstring mapPath, XMFLOAT3 position, XMFLOAT3 scale)
{
	LoadMap(mapPath);

	GetTransform()->SetPosition(position);
	GetTransform()->SetScale(scale);
	m_tileScaleInt = scale.x;

	InitMap();
}

void TileMap::InitTilePosition(Tile* tile, int row, int column) // Give tiles positions in the world relative to the TileMaps position
{
	XMFLOAT2 tileScale = XMFLOAT2(m_transform->GetScale().x * 2.0f, m_transform->GetScale().y * 2.0f);

	XMFLOAT2 pos = MathHelper::Minus(XMFLOAT2(m_transform->GetPosition().x, m_transform->GetPosition().y), XMFLOAT2(m_width * tileScale.x * 0.5f, m_height * tileScale.y * 0.5f));	//Offset to min tile can be

	//Offsetting by half tile scale and then positioning based on row and column
	pos = MathHelper::Plus(pos, XMFLOAT2(tileScale.x * 0.5f, tileScale.y * 0.5f));
	pos = MathHelper::Plus(pos, XMFLOAT2(row * tileScale.x, column * tileScale.y));

	tile->GetTransform()->SetPosition(XMFLOAT3(pos.x, pos.y, 0));
}

void TileMap::LoadMap(wstring path) // Load data for the map from a given path
{
	int count = 0;
	string parameter;
	string line;

	string data = "";
	vector<string> dataVec = {};

	ifstream mapFile(path);
	if (mapFile.is_open()) // Loads data by reading each line
	{
		while (getline(mapFile, line))
		{
			count++;

			switch (count)
			{
				case(1):
				{
					parameter = "<DIMENSIONS>";
					break;
				}
				case(2):
				{
					parameter = "<SPRITES>";
					break;
				}
				case(3):
				{
					parameter = "<ANIMATIONS>";
					break;
				}
				case(4):
				{
					parameter = "<LAYOUT>";
					break;
				}
			}

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

					data = "";
					dataVec.clear();

					m_tileSpriteIndex = vector<int>( m_width * m_height );
				}
				else
				{
					LOG("ERROR WHEN LOADING TILE MAP - PARAMETER 'DIMENSIONS' NOT FOUND");
				}
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
							dataVec.push_back(data);
							data.clear();
						}
						else
						{
							if (line[i] != ' ')
								data.push_back(line[i]);
						}
					}

					for (int j = 0; j < dataVec.size(); j++)
					{
						m_spritePaths.push_back(dataVec[j]);
					}

					data = "";
					dataVec.clear();
				}
				else
				{
					LOG("ERROR WHEN LOADING TILE MAP - PARAMETER 'SPRITES' NOT FOUND");
				}
			}
			
			//Animations
			if (parameter == "<ANIMATIONS>")
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

					for (int j = 0; j < dataVec.size(); j++)
					{
						m_animPaths.push_back(dataVec[j]);
					}

					data = "";
					dataVec.clear();
				}
				else
				{
					LOG("ERROR WHEN LOADING TILE MAP - PARAMETER 'ANIMATIONS' NOT FOUND");
				}
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
							dataVec.push_back(data);
							data.clear();
						}
						else
						{
							if (line[i] != ' ')
								data.push_back(line[i]);
						}
					}

					for (int j = 0; j < dataVec.size(); j++)
					{
						m_tileSpriteIndex[j] = stoi(dataVec[j]);
					}

					data = "";
					dataVec.clear();
				}
				else
				{
					LOG("ERROR WHEN LOADING TILE MAP - PARAMETER 'LAYOUT' NOT FOUND");
				}
			}
		}
		mapFile.close();
	}
}

void TileMap::AssignSprites() // Sets each tiles sprite or animaton based off of the layout data from a file
{
	int totalSprites = m_animPaths.size() + m_spritePaths.size();
	int count = 0;

	for (int i = 0; i < m_height; ++i)
	{
		for (int j = 0; j < m_width; ++j)
		{
			if (m_tileSpriteIndex[count] < m_spritePaths.size())
			{
				if (m_tileSpriteIndex[count] > m_spritePaths.size() + m_animPaths.size())
				{
					LOG("ERROR WHEN LOADING ASSIGNING TILE SPRITE - INDEX OUT OF RANGE");
				}
				else
				{
					try
					{
						// String to wString conversion code from RipTutorial.com
						std::string stringPath = m_spritePaths[m_tileSpriteIndex[count]];
						std::wstring wStringPath;

						// convert to wstring
						wStringPath = L"Resources\\Sprites\\" + std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(stringPath);

						m_tiles[i][j]->SetAlbedo(wStringPath);
					}
					catch(...)
					{
						m_tiles[i][j]->SetAlbedo(DEFAULT_TILE);
					}

					count++;
				}
			}
			else
			{
				if (m_tileSpriteIndex[count] > m_spritePaths.size() + m_animPaths.size())
				{
					LOG("ERROR WHEN LOADING ASSIGNING TILE ANIMATION - INDEX OUT OF RANGE");
				}
				else
				{
					// String to wString conversion code from RipTutorial.com
					std::string stringPath = m_animPaths[m_tileSpriteIndex[count] - m_spritePaths.size()];
					std::wstring wStringPath;

					// convert to wstring
					wStringPath = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(stringPath);

					m_tiles[i][j]->AddAnimation("TestAnim", wStringPath);
					count++;
				}
			}
		}
	}
}

void TileMap::CreateTile(int row, int column)
{
	m_tiles[row][column] = GameManager::GetInstance()->CreateGameObject<Tile>("Tile_" + to_string(row) + "_" + to_string(column));

	XMFLOAT3 scale = GetTransform()->GetScale();

	m_tiles[row][column]->GetTransform()->SetScale(scale);

	InitTilePosition(m_tiles[row][column], row, column);
}

Tile* TileMap::GetTileFromWorldPos(XMFLOAT2 pos) // Takes a set of coordinates and finds if a tile is there
{
	XMFLOAT2 tileMapPos = XMFLOAT2(m_transform->GetPosition().x, m_transform->GetPosition().y);

	XMFLOAT2 relativePos = MathHelper::Minus(pos, tileMapPos);

	XMFLOAT2 tileScale = XMFLOAT2(m_transform->GetScale().x * 2.0f, m_transform->GetScale().y * 2.0f);

	relativePos = MathHelper::Plus(relativePos, XMFLOAT2(m_width * tileScale.x * 0.5f, m_height * tileScale.y * 0.5f));

	int row = (relativePos.x / (int)tileScale.x) - 1;
	int column = ((int)relativePos.y % (int)tileScale.y) - 1;

	if (row >= m_width || column >= m_height)
	{
		LOG("Tried to get tilemap using position that isn't in the tilemap");

		return nullptr;
	}

	return m_tiles[row][column];
}

Tile* TileMap::GetTileFromMapPos(int x, int y) // Returns the tile in the given TileMap coordinates
{
	if (x < m_width && x >= 0 && y < m_height && y >= 0)
	{
		return m_tiles[x][y];
	}
	else
	{
		LOG("ERROR - INVALID TILEMAP COORDINATE");
		return nullptr;
	}
}

void TileMap::SetTileAtWorldPos(XMFLOAT2 worldPos, Tile* newTile)
{


}

void TileMap::SetTileAtMapPos(int posX, int posY, Tile* newTile)
{
	m_tiles[posY][posX] = newTile;
}

XMFLOAT3 TileMap::GetScale()
{
	return XMFLOAT3();
}

void TileMap::SetScale(XMFLOAT3 newScale)
{
	GetTransform()->SetScale(newScale);
}

void TileMap::SetPassable(int x, int y, bool passable)
{
	m_tiles[x][y]->SetPassable(passable);
}

void TileMap::SetPassable(Tile tile, bool passable)
{
	tile.SetPassable(passable);
}