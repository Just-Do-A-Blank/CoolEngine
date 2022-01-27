#pragma once

#if EDITOR

#include "ToolBase.h"

#include "Engine/Managers/Events/EventObserver.h"

class TileMap;
class Tile;

class TileMapTool : public ToolBase, public Observer
{
public:

	virtual void Init(ID3D11Device* pdevice);
	void Render() override;
	void Update() override;

	void Handle(Event* e) override;

protected:


private:
	void CreateSelectDimensionsUI();

	TileMap* m_ptileMap = nullptr;

	bool m_selectingDimensions = true;

	int m_tileMapWidth = 0;
	int m_tileMapHeight = 0;

	float m_tileDimensions = 0;

	Tile* m_pselectedTile = nullptr;
};

#endif