#pragma once

#if EDITOR

#include "ToolBase.h"

#include "Engine/Managers/Events/EventObserver.h"
#include "Engine/EditorUI/TileMapContentBrowser.h"

class TileMap;
class Tile;
class EditorCameraGameObject;
class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;

enum class ToolMode
{
	TEXTURE = 0,
	LAYER,
	DELETE_TILE,
	SELECT,

	COUNT
};

class TileMapTool : public ToolBase, public Observer
{
public:

	virtual void Init(ID3D11Device* pdevice);
	void Render() override;
	void Update() override;
	void Destroy() override;

	void Handle(Event* e) override;

protected:
	void MouseButtonPressed(MouseButtonPressedEvent* e);
	void MouseButtonReleased(MouseButtonReleasedEvent* e);

private:
	void CreateSelectDimensionsUI();

	TileMap* m_ptileMap = nullptr;
	EditorCameraGameObject* m_pcamera = nullptr;

	bool m_selectingDimensions = true;
	bool m_lmbPressed = false;

	int m_tileMapWidth = 0;
	int m_tileMapHeight = 0;

	int m_paintLayer = 0;

	float m_tileDimensions = 0;

	std::wstring m_relativePath = L"";

	ID3D11ShaderResourceView* m_ppaintSRV = nullptr;

	DirectX::XMINT2 m_selectedTile = DirectX::XMINT2(-1, -1);
	DirectX::XMINT2 m_CopiedTile = DirectX::XMINT2(-1, -1);

	ContentBrowser m_contentBrowser;

	ToolMode m_toolMode = ToolMode::SELECT;
};

#endif