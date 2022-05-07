#if EDITOR

#include "TileMapTool.h"

#include "Engine/Includes/IMGUI/imgui.h"
#include "Engine/Includes/IMGUI/imgui_impl_win32.h"
#include "Engine/Includes/IMGUI/imgui_impl_dx11.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Tools/TileMapTool.h"
#include "Engine/TileMap/TileMap/TileMap.h"
#include "Engine/Managers/Events/EventManager.h"
#include "Engine/GameObjects/CameraGameObject.h"

void TileMapTool::Init(ID3D11Device* pdevice)
{
	ToolBase::Init(pdevice);

	EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
}

void TileMapTool::Render()
{
	if (m_selectingDimensions == true)
	{
		CreateSelectDimensionsUI();
	}
	else
	{
		ImGui::Begin("Properties");

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		if (m_selectedTile.x != -1 && m_selectedTile.y != -1)
		{
			Tile* ptile = nullptr;

			if (m_ptileMap->GetTileFromMapPos(m_selectedTile.x, m_selectedTile.y, ptile) == true && ptile != nullptr)
			{
				ptile->ShowEngineUI();
			}
		}

		ImGui::End();

		ImGui::Begin("Master");

		m_ptileMap->CreateEngineUI();

		ImGui::End();
	}
}

void TileMapTool::Update()
{
	if (m_selectingDimensions == true)
	{
		return;
	}
}

void TileMapTool::Handle(Event* e)
{
	if (m_selectingDimensions == true)
	{
		return;
	}

	if (((MouseButtonPressedEvent*)e)->GetButton() == VK_LBUTTON)
	{
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(*GraphicsManager::GetInstance()->GetHWND(), &point);

		DirectX::XMFLOAT2 relativePos = EditorUI::GetViewportPosition();
		relativePos = DirectX::XMFLOAT2(point.x - relativePos.x, point.y - relativePos.y);

		float x = ((2.0f * relativePos.x) / GraphicsManager::GetInstance()->GetWindowDimensions().x) - 1.0f;
		float y = 1.0f - ((2.0f * relativePos.y) / GraphicsManager::GetInstance()->GetWindowDimensions().y);

		XMFLOAT2 clickPosWorld;

		XMStoreFloat2(&clickPosWorld, XMVector2Transform(XMVectorSet(x, y, 0, 0), XMMatrixInverse(nullptr, XMLoadFloat4x4(&GameManager::GetInstance()->GetCamera()->GetViewProjection()))));

		Tile* ptile;

		int row = -1;
		int column = -1;

		if (m_ptileMap->GetTileFromWorldPos(clickPosWorld, ptile, &row, &column) == false)
		{
			return;
		}

		if (ptile == nullptr)
		{
			m_ptileMap->CreateTile(row, column, ptile);
		}

		m_selectedTile = DirectX::XMINT2(row, column);
	}
}

void TileMapTool::CreateSelectDimensionsUI()
{
	ImGui::Begin("Tile Map Properties");

	EditorUI::DragInt("Width", m_tileMapWidth);
	EditorUI::DragInt("Height", m_tileMapHeight);
	EditorUI::DragFloat("Tile Dimensions", m_tileDimensions);

	if (ImGui::Button("Create") == true)
	{
		m_ptileMap = new TileMap(m_tileMapWidth, m_tileMapHeight, "TileMap", XMFLOAT3(0, 0, 0), m_tileDimensions);

		m_selectingDimensions = false;
	}

	ImGui::End();
}

#endif