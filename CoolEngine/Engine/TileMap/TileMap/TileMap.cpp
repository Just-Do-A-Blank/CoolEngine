#include "TileMap.h"

#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Includes/json.hpp"
#include "Engine/Graphics/AnimationStateMachine.h"
#include "Engine/Graphics/AnimationState.h"
#include "Engine/Physics/Collision.h"

using namespace nlohmann;

TileMap::TileMap() : RenderableGameObject()
{
	m_gameObjectType |= GameObjectType::TILE_MAP;

#if EDITOR
	Tile::s_ptileMap = this;
#endif
}

TileMap::TileMap(string identifier, CoolUUID uuid) : RenderableGameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::TILE_MAP;

#if EDITOR
	Tile::s_ptileMap = this;
#endif
}

TileMap::TileMap(const nlohmann::json& data, CoolUUID uuid) : RenderableGameObject(data, uuid)
{
	m_gameObjectType |= GameObjectType::TILE_MAP;

#if EDITOR
	Tile::s_ptileMap = this;
#endif

	if (data.contains("MapName"))
	{
		m_tileMapName = data["MapName"];
	}
	else
	{
		m_tileMapName = "";
	}

	m_editorTileMapName = m_tileMapName;

	XMFLOAT3 position = XMFLOAT3(data["Position"][0], data["Position"][1], data["Position"][2]);

	if (data.contains("PathFinding") == true)
	{
		m_pathfindingMesh = data["PathFinding"];
	}
	else
	{
		m_pathfindingMesh = false;
	}

	Init(m_tileMapName, position);
}

TileMap::TileMap(TileMap const& other) : RenderableGameObject(other)
{
#if EDITOR
	Tile::s_ptileMap = this;
#endif

	m_tileMapName = other.m_tileMapName;
	m_editorTileMapName = m_tileMapName;

	XMFLOAT3 position = other.GetTransform()->GetLocalPosition();

	Init(m_tileMapName, position);
}

TileMap::TileMap(string mapName, XMFLOAT3 position, string identifier, CoolUUID uuid) : RenderableGameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::TILE_MAP;

#if EDITOR
	Tile::s_ptileMap = this;
#endif

	m_tileMapName = mapName;
	m_editorTileMapName = m_tileMapName;

	Init(mapName, position);
}

TileMap::TileMap(int width, int height, string identifier, CoolUUID uuid, XMFLOAT3 position, float tileDimensions) : RenderableGameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::TILE_MAP;

#if EDITOR
	Tile::s_ptileMap = this;
#endif

	Init(width, height, position, tileDimensions);
}

TileMap::~TileMap()
{
	for (int i = 0; i < m_height; i++)
	{
		for (int j = 0; j < m_width; j++)
		{
			if (m_tiles[i][j] != nullptr)
			{
				delete m_tiles[i][j];
				m_tiles[i][j] = nullptr;
			}
		}
	}

	m_tiles.clear(); // Run first to allow each tile to release its memory, but this does not release the memory for the vector
	m_tiles.resize(0);

	m_spritePaths.resize(0);
	m_animPaths.resize(0);
}

void TileMap::Update()
{
	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			if (m_tiles[i][j] != nullptr)
			{
				m_tiles[i][j]->Update();
			}
		}
	}

	Scene* currentScene = GameManager::GetInstance()->GetCurrentScene();
	if (currentScene)
	{
		for (int i = 0; i < m_width; ++i)
		{
			Collision::Update(currentScene->GetSceneGraph()->GetAllNodeObjects(), m_tiles[i]);
		}
	}
}

void TileMap::EditorUpdate()
{
	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			if (m_tiles[i][j] != nullptr)
			{
				m_tiles[i][j]->Update();
			}
		}
	}
}

void TileMap::Render(RenderStruct& renderStruct)
{
	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			if (m_tiles[i][j] != nullptr)
			{
				m_tiles[i][j]->Render(renderStruct);
			}
		}
	}
}

void TileMap::InitMap() // Create and store tiles in m_Tiles
{
	GameManager* pGameManager = GameManager::GetInstance();

	XMFLOAT3 scale = GetTransform()->GetLocalScale();

	m_tiles.resize(m_width);

	int ID = 0;
	for (int i = 0; i < m_width; ++i)
	{
		for (int j = 0; j < m_height; ++j)
		{
			if (j == 0)
			{
				m_tiles[i].resize(m_height);
			}

			m_tiles[i][j] = nullptr;
		}
	}
}

void TileMap::InitTilePosition(Tile* tile, int row, int column) // Give tiles positions in the world relative to the TileMaps position
{
	XMFLOAT2 tileScale = XMFLOAT2(m_transform->GetLocalScale().x * 2.0f, m_transform->GetLocalScale().y * 2.0f);

	XMFLOAT2 pos = MathHelper::Minus(XMFLOAT2(m_transform->GetLocalPosition().x, m_transform->GetLocalPosition().y), XMFLOAT2(m_width * tileScale.x * 0.5f, m_height * tileScale.y * 0.5f));	//Offset to min tile can be

	//Offsetting by half tile scale and then positioning based on row and column
	pos = MathHelper::Plus(pos, XMFLOAT2(tileScale.x * 0.5f, tileScale.y * 0.5f));
	pos = MathHelper::Plus(pos, XMFLOAT2(row * tileScale.x, column * tileScale.y));

	tile->GetTransform()->SetLocalPosition(XMFLOAT3(pos.x, pos.y, 0));
}

bool TileMap::Load(string path) // Load data for the map from a given path
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
	m_transform->SetLocalScale(scale);

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
		m_tiles[i].resize(m_height);

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

				m_tiles[i][j]->SetSpriteIndex(spriteIndex);
				m_tiles[i][j]->SetAnimIndex(animIndex);

				m_tiles[i][j]->SetLayer(tileLayer);
				m_tiles[i][j]->SetIsPassable(tilePassable);

				if (spriteIndex != -1)
				{
					m_tiles[i][j]->SetAlbedo(m_spritePaths[spriteIndex]);
				}

				if (animIndex != -1)
				{
					AnimationState* pstate = new AnimationState();
					pstate->SetName("default");
					pstate->SetAnimation(m_animPaths[animIndex]);

					m_tiles[i][j]->AddAnimationState("default", pstate, true);
				}
			}
		}
	}

	return true;
}

bool TileMap::Save(string path)
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
				jsonOutput["TileSpriteIndexes"].push_back({ m_tiles[i][j]->GetSpriteIndex() });
				jsonOutput["TileAnimIndexes"].push_back({ m_tiles[i][j]->GetAnimIndex() });

				jsonOutput["TileLayer"].push_back({ m_tiles[i][j]->GetLayer() });
				jsonOutput["TilePassable"].push_back({ m_tiles[i][j]->GetIsPassable() });
			}

		}
	}

	jsonOutput["TileMapScale"].push_back({ m_transform->GetLocalScale().x, m_transform->GetLocalScale().y, m_transform->GetLocalScale().z });

	outFile << jsonOutput;

	outFile.close();

	return true;
}

bool TileMap::GetCoordsFromWorldPos(int* prow, int* pcolumn, const XMFLOAT2& pos)
{
	XMFLOAT2 relativePos = MathHelper::Minus(pos, XMFLOAT2(m_transform->GetLocalPosition().x, m_transform->GetLocalPosition().y));

	XMFLOAT2 tileScale = XMFLOAT2(m_transform->GetLocalScale().x * 2.0f, m_transform->GetLocalScale().y * 2.0f);

	relativePos = MathHelper::Plus(relativePos, XMFLOAT2(m_width * tileScale.x * 0.5f, m_height * tileScale.y * 0.5f));

	float row;
	float column;

	if (relativePos.x < 0 || relativePos.y < 0)
	{
		row = -1;
		column = -1;
	}
	else
	{
		row = (relativePos.x / tileScale.x);
		column = (relativePos.y / tileScale.y);
	}

	*prow = row;
	*pcolumn = column;

	if (row >= (float)m_width || column >= (float)m_height || row < 0 || column < 0)
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

	m_tiles[row][column] = new Tile("", CoolUUID());
	m_tiles[row][column]->Init("Tile_" + to_string(row) + "_" + to_string(column), CoolUUID());

	XMFLOAT3 scale = GetTransform()->GetLocalScale();

	m_tiles[row][column]->GetTransform()->SetLocalScale(scale);

	InitTilePosition(m_tiles[row][column], row, column);

	ptile = m_tiles[row][column];

	ptile->GetTransform()->UpdateMatrix();

	return true;
}

void TileMap::AddSpritePath(Tile* ptile, wstring& path)
{
	for (int i = 0; i < m_spritePaths.size(); ++i)
	{
		if (m_spritePaths[i] == path)
		{
			ptile->SetSpriteIndex(i);

			return;
		}
	}

	m_spritePaths.push_back(path);

	ptile->SetSpriteIndex(m_spritePaths.size() - 1);
}

void TileMap::AddAnimPath(Tile* ptile, wstring& path)
{
	for (int i = 0; i < m_animPaths.size(); ++i)
	{
		if (m_animPaths[i] == path)
		{
			ptile->SetAnimIndex(i);

			return;
		}
	}

	m_animPaths.push_back(path);

	ptile->SetAnimIndex(m_animPaths.size() - 1);
}

void TileMap::DeleteTile(int row, int column)
{
	if (m_tiles[row][column] == nullptr)
	{
		return;
	}

	delete m_tiles[row][column];
	m_tiles[row][column] = nullptr;
}

#if EDITOR
void TileMap::CreateEngineUI()
{
	EditorUI::InputText("Tile Map Name", m_editorTileMapName);

	ImGui::Spacing();

	if (ImGui::Button("Save") == true)
	{
		if (m_editorTileMapName == "")
		{
			LOG("Tried to save the tile map but didn't enter a name!");
		}
		else
		{
			string filepath = GameManager::GetInstance()->GetWorkingDirectory() + "\\Resources\\Levels\\TileMaps\\" + m_editorTileMapName + ".json";

			if (Save(filepath) == true)
			{
				LOG("Tile map saved!");

				m_tileMapName = m_editorTileMapName;
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
		if (m_editorTileMapName == "")
		{
			LOG("Tried to load a tile map but didn't enter a name!");
		}
		else
		{
			string filepath = GameManager::GetInstance()->GetWorkingDirectory() + "\\Resources\\Levels\\TileMaps\\" + m_editorTileMapName + ".json";

			for (int i = 0; i < m_width; ++i)
			{
				for (int j = 0; j < m_height; ++j)
				{
					if (m_tiles[i][j] != nullptr)
					{
						delete m_tiles[i][j];
						m_tiles[i][j] = nullptr;
					}
				}

				m_tiles[i].clear();
			}

			m_spritePaths.clear();
			m_animPaths.clear();

			if (Load(filepath) == true)
			{
				LOG("Tile map loaded!");

				m_tileMapName = m_editorTileMapName;
			}
			else
			{
				LOG("Tile map failed to load!");
			}
		}
	}

	ImGui::Spacing();

	if (EditorUI::Checkbox("Show Colliders", m_showColliders) == true)
	{
		for (int i = 0; i < m_width; ++i)
		{
			for (int j = 0; j < m_height; ++j)
			{
				if (m_tiles[i][j] != nullptr && m_tiles[i][j]->GetIsPassable() == false)
				{
					m_tiles[i][j]->GetShape()->SetIsRendered(m_showColliders);
				}
			}
		}
	}

	ImGui::Spacing();

	EditorUI::Checkbox("Pathfinding", m_pathfindingMesh);

	ImGui::Spacing();
	ImGui::Separator();
}
#endif

void TileMap::Init(int width, int height, XMFLOAT3 position, float tileDimensions)
{
	m_width = width;
	m_height = height;

	m_totalTiles = m_width * m_height;

	XMFLOAT3 scale = XMFLOAT3(tileDimensions, tileDimensions, tileDimensions);

	m_transform->SetLocalScale(scale);

	m_transform->SetLocalPosition(position);

	InitMap();
}

void TileMap::Init(string mapPath, XMFLOAT3 position)
{
	m_transform->SetLocalPosition(position);

	string filepath = GameManager::GetInstance()->GetWorkingDirectory() + "\\Resources\\Levels\\TileMaps\\" + mapPath + ".json";

	if (Load(filepath) == true && m_pathfindingMesh == true)
	{
		Pathfinding::GetInstance()->Initialize(this);
	}
}

void TileMap::Serialize(nlohmann::json& data)
{
	RenderableGameObject::Serialize(data);

	data["MapName"] = m_tileMapName;
	data["Position"] = { m_transform->GetLocalPosition().x ,m_transform->GetLocalPosition().y ,m_transform->GetLocalPosition().z };
	data["PathFinding"] = m_pathfindingMesh;
}

std::string TileMap::GetMapName() const
{
	return m_tileMapName;
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
	return m_transform->GetLocalScale();
}

void TileMap::SetTileScale(XMFLOAT3 newScale)
{
	m_transform->SetLocalScale(newScale);
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
