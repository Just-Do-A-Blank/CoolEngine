#include "TileMap.h"

#include "Engine/EditorUI/EditorUI.h"
#include  "Engine/Includes/json.hpp"

using namespace nlohmann;

TileMap::TileMap() : RenderableGameObject()
{
#if EDITOR
	Tile::s_ptileMap = this;
#endif
}

TileMap::TileMap(string identifier, CoolUUID uuid) : RenderableGameObject(identifier, uuid)
{
#if EDITOR
	Tile::s_ptileMap = this;
#endif
}

TileMap::TileMap(wstring mapPath, XMFLOAT3 position, string identifier, CoolUUID uuid) : RenderableGameObject(identifier, uuid)
{
#if EDITOR
	Tile::s_ptileMap = this;
#endif

	m_transform->SetPosition(position);

	Load(mapPath);
}

TileMap::TileMap(int width, int height, string identifier, CoolUUID uuid, XMFLOAT3 position, float tileDimensions) : RenderableGameObject(identifier, uuid)
{
#if EDITOR
	Tile::s_ptileMap = this;
#endif

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

	XMFLOAT3 scale = GetTransform()->GetWorldScale();

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
	XMFLOAT2 tileScale = XMFLOAT2(m_transform->GetWorldScale().x * 2.0f, m_transform->GetWorldScale().y * 2.0f);

	XMFLOAT2 pos = MathHelper::Minus(XMFLOAT2(m_transform->GetWorldPosition().x, m_transform->GetWorldPosition().y), XMFLOAT2(m_width * tileScale.x * 0.5f, m_height * tileScale.y * 0.5f));	//Offset to min tile can be

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

	XMFLOAT3 scale = XMFLOAT3(jsonData["TileMapScale"][0][0], jsonData["TileMapScale"][0][1], jsonData["TileMapScale"][0][2]);
	m_transform->SetScale(scale);

	m_width = jsonData["Dimensions"][0];
	m_height = jsonData["Dimensions"][1];

	for (int i = 0; i < jsonData["SpritePaths"].size(); ++i)
	{
		m_spritePaths.push_back(jsonData["SpritePaths"][i]);
	}

	for (int i = 0; i < jsonData["AnimPaths"].size(); ++i)
	{
		m_animPaths.push_back(jsonData["AnimPaths"][i]);
	}

	m_tiles.resize(m_width);

	int spriteIndex = -1;
	int animIndex = -1;
	int tileLayer = -1;
	bool tilePassable = false;

	Tile* ptile;

	for (int i = 0; i < m_width; ++i)
	{
		m_tiles[i].resize(m_width);

		for (int j = 0; j < m_height; ++j)
		{
			spriteIndex = jsonData["TileSpriteIndexes"][(i * m_width) + j][0];
			animIndex = jsonData["TileAnimIndexes"][(i * m_width) + j][0];
			tileLayer = jsonData["TileLayer"][(i * m_width) + j][0];
			tilePassable = jsonData["TilePassable"][(i * m_width) + j][0];

			if (spriteIndex == -1 && animIndex == -1)
			{
				m_tiles[i][j] = nullptr;
			}
			else
			{
				CreateTile(i, j, ptile);

#if TILE_MAP_TOOL
				m_tiles[i][j]->SetSpriteIndex(spriteIndex);
				m_tiles[i][j]->SetAnimIndex(animIndex);
#endif

				m_tiles[i][j]->SetLayer(tileLayer);
				m_tiles[i][j]->SetIsPassable(tilePassable);

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

	for (int i = 0; i < m_spritePaths.size(); ++i)
	{
		jsonOutput["SpritePaths"].push_back(m_spritePaths[i]);
	}

	for (int i = 0; i < m_animPaths.size(); ++i)
	{
		jsonOutput["AnimPaths"].push_back(m_animPaths[i]);
	}

	for (int i = 0; i < m_width; ++i)
	{
		for (int j = 0; j < m_height; ++j)
		{
			if (m_tiles[i][j] == nullptr)
			{
				jsonOutput["TileSpriteIndexes"].push_back({ -1 });
				jsonOutput["TileAnimIndexes"].push_back({ -1 });
				jsonOutput["TileLayer"].push_back({ -1 });
				jsonOutput["TilePassable"].push_back({ false });
			}
			else
			{
#if TILE_MAP_TOOL
				jsonOutput["TileSpriteIndexes"].push_back({ m_tiles[i][j]->GetSpriteIndex() });
				jsonOutput["TileAnimIndexes"].push_back({ m_tiles[i][j]->GetAnimIndex() });
#endif

				jsonOutput["TileLayer"].push_back({ m_tiles[i][j]->GetLayer() });
				jsonOutput["TilePassable"].push_back({ m_tiles[i][j]->GetIsPassable() });
			}

		}
	}

	jsonOutput["TileMapScale"].push_back({ m_transform->GetWorldScale().x, m_transform->GetWorldScale().y, m_transform->GetWorldScale().z });

	outFile << jsonOutput;

	outFile.close();

	return true;
}

bool TileMap::GetCoordsFromWorldPos(int* prow, int* pcolumn, const XMFLOAT2& pos)
{
	XMFLOAT2 relativePos = MathHelper::Minus(pos, XMFLOAT2(m_transform->GetWorldPosition().x, m_transform->GetWorldPosition().y));

	XMFLOAT2 tileScale = XMFLOAT2(m_transform->GetWorldScale().x * 2.0f, m_transform->GetWorldScale().y * 2.0f);

	relativePos = MathHelper::Plus(relativePos, XMFLOAT2(m_width * tileScale.x * 0.5f, m_height * tileScale.y * 0.5f));

	if (relativePos.x < 0 || relativePos.y < 0)
	{
		*prow = -1;
		*pcolumn = -1;
	}
	else
	{
		*prow = (relativePos.x / (int)tileScale.x);
		*pcolumn = ((int)relativePos.y / (int)tileScale.y);
	}


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

	XMFLOAT3 scale = GetTransform()->GetWorldScale();

	m_tiles[row][column]->GetTransform()->SetScale(scale);

	InitTilePosition(m_tiles[row][column], row, column);

	ptile = m_tiles[row][column];

	return true;
}

void TileMap::AddSpritePath(Tile* ptile, wstring& path)
{
	for (int i = 0; i < m_spritePaths.size(); ++i)
	{
		if (m_spritePaths[i] == path)
		{
#if TILE_MAP_TOOL
			ptile->SetSpriteIndex(i);
#endif

			return;
		}
	}

	m_spritePaths.push_back(path);

#if TILE_MAP_TOOL
	ptile->SetSpriteIndex(m_spritePaths.size() - 1);
#endif
}

void TileMap::AddAnimPath(Tile* ptile, wstring& path)
{
	for (int i = 0; i < m_animPaths.size(); ++i)
	{
		if (m_animPaths[i] == path)
		{
#if TILE_MAP_TOOL
			ptile->SetAnimIndex(i);
#endif

			return;
		}
	}

	m_animPaths.push_back(path);

#if TILE_MAP_TOOL
	ptile->SetAnimIndex(m_animPaths.size() - 1);
#endif
}

#if EDITOR
void TileMap::CreateEngineUI()
{
	ImGui::Separator();
	ImGui::Spacing();

	EditorUI::InputText("Tile Map Name", m_tileMapName);

	ImGui::Spacing();

	if (ImGui::Button("Save") == true)
	{
		if (m_tileMapName == "") 
		{
			LOG("Tried to save the tile map but didn't enter a name!");
		}
		else
		{
			wstring filepath = GameManager::GetInstance()->GetWideWorkingDirectory() + L"\\Resources\\Levels\\TileMaps\\" + wstring(m_tileMapName.begin(), m_tileMapName.end()) + L".json";

			if (Save(filepath) == true)
			{
				LOG("Tile map saved!");
			}
			else
			{
				LOG("Tile map failed to save!");
			}
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Load") == true)
	{
		if (m_tileMapName == "")
		{
			LOG("Tried to load a tile map but didn't enter a name!");
		}
		else
		{
			wstring filepath = GameManager::GetInstance()->GetWideWorkingDirectory() + L"\\Resources\\Levels\\TileMaps\\" + wstring(m_tileMapName.begin(), m_tileMapName.end()) + L".json";

			for (int i = 0; i < m_width; ++i)
			{
				for (int j = 0; j < m_height; ++j)
				{
					if (m_tiles[i][j] != nullptr)
					{
						GameManager::GetInstance()->DeleteGameObject(m_tiles[i][j], m_tiles[i][j]->GetIdentifier());
					}
				}

				m_tiles[i].clear();
			}

			m_spritePaths.clear();
			m_animPaths.clear();

			if (Load(filepath) == true)
			{
				LOG("Tile map loaded!");
			}
			else
			{
				LOG("Tile map failed to load!");
			}
		}
	}

	ImGui::Spacing();
	ImGui::Separator();
}
#endif

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
	XMFLOAT2 relativePos = MathHelper::Minus(worldPos, XMFLOAT2(m_transform->GetWorldPosition().x, m_transform->GetWorldPosition().y));

	XMFLOAT2 tileScale = XMFLOAT2(m_transform->GetWorldScale().x * 2.0f, m_transform->GetWorldScale().y * 2.0f);

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
	return m_transform->GetWorldScale();
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