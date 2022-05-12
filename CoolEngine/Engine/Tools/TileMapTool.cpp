#if EDITOR

#include "TileMapTool.h"

#include "Engine/Includes/IMGUI/imgui.h"
#include "Engine/Includes/IMGUI/imgui_impl_win32.h"
#include "Engine/Includes/IMGUI/imgui_impl_dx11.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Tools/TileMapTool.h"
#include "Engine/TileMap/TileMap/TileMap.h"
#include "Engine/Managers/Events/EventManager.h"
#include "Engine/GameObjects/TileMapCameraGameObject.h"
#include "Engine/Helpers/Inputs.h"

#include "Engine/Managers/Events/MouseEvents.h"

void TileMapTool::Init(ID3D11Device* pdevice)
{
	ToolBase::Init(pdevice);

	EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);

	//Create camera
	XMFLOAT3 cameraPos = XMFLOAT3(0, 0, -5);
	XMFLOAT3 cameraForward = XMFLOAT3(0, 0, 1);
	XMFLOAT3 cameraUp = XMFLOAT3(0, 1, 0);

	float windowWidth = GraphicsManager::GetInstance()->GetWindowDimensions().x;
	float windowHeight = GraphicsManager::GetInstance()->GetWindowDimensions().y;

	float nearDepth = 0.01f;
	float farDepth = 1000.0f;

	CoolUUID uuid;
	m_pcamera = new TileMapCameraGameObject("Camera", uuid);
	m_pcamera->Initialize(cameraPos, cameraForward, cameraUp, windowWidth, windowHeight, nearDepth, farDepth);

	GameManager::GetInstance()->SetCamera(m_pcamera);
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

		ImGui::Begin("Master", nullptr, ImGuiWindowFlags_MenuBar);

		CreateMenuBar();

		m_ptileMap->CreateEngineUI();

		ImGui::End();

		m_contentBrowser.Draw();
	}
}

void TileMapTool::Update()
{
	if (m_selectingDimensions == true)
	{
		return;
	}

	m_pcamera->Update();

	if (Inputs::GetInstance()->IsKeyPressed(VK_CONTROL) && Inputs::GetInstance()->IsKeyPressed('C'))
	{
		m_CopiedTile = m_selectedTile;
	}
	else if (Inputs::GetInstance()->IsKeyPressed(VK_CONTROL) && Inputs::GetInstance()->IsKeyPressed('V'))
	{
		Tile* pDestTile = nullptr;
		Tile* pSourceTile = nullptr;

		if (m_ptileMap->GetTileFromMapPos(m_selectedTile.x, m_selectedTile.y, pDestTile) &&
			m_ptileMap->GetTileFromMapPos(m_CopiedTile.x, m_CopiedTile.y, pSourceTile))
		{
			if (pSourceTile != nullptr && pDestTile != nullptr)
			{
				pDestTile->CopyTile(pSourceTile);
			}
		}
	}
}

void TileMapTool::Destroy()
{
	ToolBase::Destroy();

	EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonPressed, this);
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

		float x = ((2.0f * relativePos.x) / EditorUI::GetViewportSize().x) - 1.0f;
		float y = 1.0f - ((2.0f * relativePos.y) / EditorUI::GetViewportSize().y);

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
		CoolUUID uuid;
		m_ptileMap = new TileMap(m_tileMapWidth, m_tileMapHeight, "TileMap", uuid, XMFLOAT3(0, 0, 0), m_tileDimensions);

		m_selectingDimensions = false;
	}

	ImGui::End();
}

#endif