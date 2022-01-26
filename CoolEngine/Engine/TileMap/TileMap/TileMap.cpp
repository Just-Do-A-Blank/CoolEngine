#include "TileMap.h"

#include  "Engine/Includes/json.hpp"

using namespace nlohmann;

TileMap::TileMap() : RenderableGameObject()
{
}

TileMap::TileMap(string identifier) : RenderableGameObject(identifier)
{
}

TileMap::TileMap(wstring mapPath, XMFLOAT3 position, float tileDimensions, string identifier) : RenderableGameObject(identifier)
{
	m_transform->SetPosition(position);

	XMFLOAT3 scale = XMFLOAT3(tileDimensions, tileDimensions, tileDimensions);

	m_transform->SetScale(scale);

	Load(mapPath);

	InitMap();
}

TileMap::TileMap(int width, int height, string identifier, XMFLOAT3 position, float tileDimensions) : RenderableGameObject(identifier)
{
	m_width = width;
	m_height = height;

	m_totalTiles = m_width * m_height;

	XMFLOAT3 scale = XMFLOAT3(tileDimensions, tileDimensions, tileDimensions);

	m_transform->SetScale(scale);

	m_transform->SetPosition(position);

	InitMap();
}

TileMap::~TileMap()
{
	m_tiles.clear(); // Run first to allow each tile to release its memory, but this does not release the memory for the vector
	m_tiles.resize(0);

	m_spritePaths.resize(0);
	m_animPaths.resize(0);
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

			m_tiles[i][j] = nullptr;
		}
	}
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

bool TileMap::Load(wstring path) // Load data for the map from a given path
{
	ifstream inFile(path);

	if (inFile.is_open() == false)
	{
		LOG("Failed to load the tile map as couldn't open the file!");

		return false;
	}

	json jsonData;

	inFile >> jsonData;

	inFile.close();

	m_width = jsonData["Dimensions"][0];
	m_height = jsonData["Dimensions"][1];

	for (int i = 0; i < jsonData["SpritePaths"].size(); ++i)
	{
		m_spritePaths.push_back(jsonData["SpritePaths"][i]);
	}

	for (int i = 0; i < jsonData["AnimPaths"].size(); ++i)
	{
		m_spritePaths.push_back(jsonData["AnimPaths"][i]);
	}

	m_tiles.resize(m_width);

	int spriteIndex = -1;
	int animIndex = -1;

	Tile* ptile;

	for (int i = 0; i < m_width; ++i)
	{
		m_tiles[i].reserve(m_width);

		for (int j = 0; j < m_height; ++j)
		{
			spriteIndex = jsonData["TileIndexes"][i][j][0];
			animIndex = jsonData["TileIndexes"][i][j][1];

			if (spriteIndex == -1 && animIndex == -1)
			{
				m_tiles[i][j] = nullptr;
			}
			else
			{
				CreateTile(i, j, ptile);

				m_tiles[i][j]->SetSpriteIndex(spriteIndex);
				m_tiles[i][j]->SetAnimIndex(animIndex);

				if (spriteIndex != -1)
				{
					m_tiles[i][j]->SetAlbedo(m_spritePaths[spriteIndex]);
				}

				if (animIndex != -1)
				{
					m_tiles[i][j]->AddAnimation("default", m_animPaths[animIndex]);
					m_tiles[i][j]->PlayAnimation("default");
				}
			}
		}
	}

	return true;
}

bool TileMap::Save(wstring path)
{
	std::ofstream outFile;
	outFile.open(path);

	if (outFile.is_open() == false)
	{
		LOG("Failed to save the tile map as couldn't open the file!");

		return false;
	}

	json jsonOutput = {};
	jsonOutput["Dimensions"] = { m_width, m_height };
	jsonOutput["SpritePaths"] = {};

	for (int i = 0; i < m_spritePaths.size(); ++i)
	{
		jsonOutput["SpritePaths"].push_back(m_spritePaths[i]);
	}

	jsonOutput["AnimPaths"] = {};

	for (int i = 0; i < m_animPaths.size(); ++i)
	{
		jsonOutput["AnimPaths"].push_back(m_animPaths[i]);
	}

	jsonOutput["TileIndexes"] = {};

	for (int i = 0; i < m_width; ++i)
	{
		jsonOutput["TileIndexes"][i].push_back({});

		for (int j = 0; j < m_height; ++j)
		{
			if (m_tiles[i][j] == nullptr)
			{
				jsonOutput["TileIndexes"][i][j].push_back({ -1, -1 });
			}
			else
			{
				jsonOutput["TileIndexes"][i][j].push_back({ m_tiles[i][j]->GetSpriteIndex(), m_tiles[i][j]->GetAnimIndex() });
			}

		}
	}

	outFile << jsonOutput;

	outFile.close();

	return true;
}

bool TileMap::GetCoordsFromWorldPos(int* prow, int* pcolumn, const XMFLOAT2& pos)
{
	XMFLOAT2 relativePos = MathHelper::Minus(pos, XMFLOAT2(m_transform->GetPosition().x, m_transform->GetPosition().y));

	XMFLOAT2 tileScale = XMFLOAT2(m_transform->GetScale().x * 2.0f, m_transform->GetScale().y * 2.0f);

	relativePos = MathHelper::Plus(relativePos, XMFLOAT2(m_width * tileScale.x * 0.5f, m_height * tileScale.y * 0.5f));

	*prow = (relativePos.x / (int)tileScale.x);
	*pcolumn = ((int)relativePos.y / (int)tileScale.y);

	if (*prow >= m_width || *pcolumn >= m_height || *prow < 0 || *pcolumn < 0)
	{
		LOG("Tried to create tile but the indexes passed in are out of range!");

		return false;
	}

	return true;
}

bool TileMap::CreateTile(int row, int column, Tile*& ptile)
{
	if (row >= m_width || column >= m_height || row < 0 || column < 0)
	{
		LOG("Tried to create tile but the indexes passed in are out of range!");

		return false;
	}

	m_tiles[row][column] = GameManager::GetInstance()->CreateGameObject<Tile>("Tile_" + to_string(row) + "_" + to_string(column));

	XMFLOAT3 scale = GetTransform()->GetScale();

	m_tiles[row][column]->GetTransform()->SetScale(scale);

	InitTilePosition(m_tiles[row][column], row, column);

	ptile = m_tiles[row][column];

	return true;
}

bool TileMap::GetTileFromWorldPos(XMFLOAT2 pos, Tile*& ptile, int* prow, int* pcolumn) // Takes a set of coordinates and finds if a tile is there
{
	int row;
	int column;

	if (GetCoordsFromWorldPos(&row, &column, pos) == false)
	{
		ptile = nullptr;

		return false;
	}

	if (prow != nullptr)
	{
		*prow = row;
	}

	if (pcolumn != nullptr)
	{
		*pcolumn = column;
	}

	ptile = m_tiles[row][column];

	return true;
}

bool TileMap::GetTileFromMapPos(int x, int y, Tile*& ptile) // Returns the tile in the given TileMap coordinates
{
	if (x < m_width && x >= 0 && y < m_height && y >= 0)
	{
		ptile = m_tiles[x][y];

		return true;
	}
	else
	{
		LOG("ERROR - INVALID TILEMAP COORDINATE");

		ptile = nullptr;

		return false;
	}
}

void TileMap::SetTileAtWorldPos(XMFLOAT2 worldPos, Tile* newTile)
{
	XMFLOAT2 relativePos = MathHelper::Minus(worldPos, XMFLOAT2(m_transform->GetPosition().x, m_transform->GetPosition().y));

	XMFLOAT2 tileScale = XMFLOAT2(m_transform->GetScale().x * 2.0f, m_transform->GetScale().y * 2.0f);

	relativePos = MathHelper::Plus(relativePos, XMFLOAT2(m_width * tileScale.x * 0.5f, m_height * tileScale.y * 0.5f));

	int row = (relativePos.x / (int)tileScale.x);
	int column = ((int)relativePos.y / (int)tileScale.y);

	if (row >= m_width || column >= m_height || row < 0 || column < 0)
	{
		LOG("Tried to get tile using position that isn't covered by the tilemap");

		return;
	}

	m_tiles[row][column] = newTile;
}

void TileMap::SetTileAtMapPos(int posX, int posY, Tile* newTile)
{
	m_tiles[posY][posX] = newTile;
}

XMFLOAT3 TileMap::GetTileScale()
{
	return m_transform->GetScale();
}

void TileMap::SetTileScale(XMFLOAT3 newScale)
{
	m_transform->SetScale(newScale);
}

void TileMap::SetPassable(int x, int y, bool passable)
{
	if (x >= m_width || y >= m_height || x < 0 || y < 0)
	{
		LOG("Couldn't set passable on that tile as it is out of range!");

		return;
	}

	m_tiles[x][y]->SetIsPassable(passable);
}