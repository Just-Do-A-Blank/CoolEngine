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

	TileMap(string identifier, CoolUUID uuid);
	TileMap(const nlohmann::json& data, CoolUUID uuid);

	TileMap(TileMap const& other);

	// Load from file
	TileMap(std::string mapName, XMFLOAT3 position, string identifier, CoolUUID uuid);

	// Load from parameters
	TileMap(int width, int height, string identifier, CoolUUID uuid, XMFLOAT3 position, float tileDimensions);

	//Destructor
	virtual ~TileMap();

	void Update() override;
	void EditorUpdate() override;
	void Render(RenderStruct& renderStruct) override;

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

	void AddSpritePath(Tile* ptile, wstring& path);
	void AddAnimPath(Tile* ptile, wstring& path);

	void DeleteTile(int row, int column);

#if EDITOR
	void CreateEngineUI() override;
#endif

	void Init(int width, int height, XMFLOAT3 position, float tileDimensions);
	void Init(std::string mapPath, XMFLOAT3 position);

	void Serialize(nlohmann::json& jsonData) override;

	std::string GetMapName() const;

protected:


private:
	void InitMap();

	void InitTilePosition(Tile* tile, int row, int column);

	bool Load(string path);
	bool Save(string path);

	bool GetCoordsFromWorldPos(int* prow, int* pcolumn, const XMFLOAT2& pos);

	int	m_width = 0;
	int	m_height = 0;
	int	m_totalTiles = 0;

	int	m_tileScaleInt;
	XMFLOAT3 m_tileScale;

	std::vector<std::vector<Tile*>>	m_tiles;

	std::vector<wstring> m_spritePaths;
	std::vector<wstring> m_animPaths;

	string m_tileMapName = "";
	string m_editorTileMapName = "";

	bool m_showColliders = false;
	bool m_pathfindingMesh = false;
};
