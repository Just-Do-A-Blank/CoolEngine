#pragma once

#if EDITOR

#include "ToolBase.h"

class TileMap;

class TileMapTool : public ToolBase
{
public:

	virtual void Init(ID3D11Device* pdevice);
	void Render() override;
	void Update() override;

protected:


private:
	void CreateSelectDimensionsUI();

	TileMap* m_ptileMap = nullptr;

	bool m_selectingDimensions = true;

	int m_tileMapWidth = -1;
	int m_tileMapHeight = -1;
};

#endif