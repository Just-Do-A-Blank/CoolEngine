#if EDITOR

#include "TileMapTool.h"

#include "Engine/Includes/IMGUI/imgui.h"
#include "Engine/Includes/IMGUI/imgui_impl_win32.h"
#include "Engine/Includes/IMGUI/imgui_impl_dx11.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Tools/TileMapTool.h"
#include "Engine/TileMap/TileMap/TileMap.h"
#include "Engine/Managers/Events/EventManager.h"
#include "Engine/GameObjects/EditorCameraGameObject.h"
#include "Engine/Helpers/Inputs.h"
#include "Engine/Managers/Events/MouseEvents.h"

#include "Engine/Managers/Events/MouseEvents.h"

void TileMapTool::Init(ID3D11Device* pdevice)
{
	ToolBase::Init(pdevice);

	EventManager::Instance()->AddClient(EventType::MouseButtonPressed, this);
	EventManager::Instance()->AddClient(EventType::MouseButtonReleased, this);

	//Create camera
	XMFLOAT3 cameraPos = XMFLOAT3(0, 0, -5);
	XMFLOAT3 cameraForward = XMFLOAT3(0, 0, 1);
	XMFLOAT3 cameraUp = XMFLOAT3(0, 1, 0);

	float windowWidth = GraphicsManager::GetInstance()->GetWindowDimensions().x;
	float windowHeight = GraphicsManager::GetInstance()->GetWindowDimensions().y;

	float nearDepth = 0.01f;
	float farDepth = 1000.0f;

	CoolUUID uuid;
	m_pcamera = new EditorCameraGameObject("Camera", uuid);
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

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Back"))
			{
				Destroy();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Texture", m_toolMode != ToolMode::TEXTURE))
			{
				m_toolMode = ToolMode::TEXTURE;

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Layer", m_toolMode != ToolMode::LAYER))
			{
				m_toolMode = ToolMode::LAYER;

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Select", m_toolMode != ToolMode::SELECT))
			{
				m_toolMode = ToolMode::SELECT;

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Delete Tile", m_toolMode != ToolMode::DELETE_TILE))
			{
				m_toolMode = ToolMode::DELETE_TILE;

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

        auto layerParameters = EditorUIIntParameters();
        layerParameters.m_minValue = 0;
        layerParameters.m_maxValue = GraphicsManager::GetInstance()->GetNumLayers() - 1;

		switch (m_toolMode)
		{
		case ToolMode::LAYER:
			EditorUI::DragInt("Layer", m_paintLayer, layerParameters);
			break;

		case ToolMode::TEXTURE:
			EditorUI::Texture("Texture", m_relativePath, m_ppaintSRV);
			break;
		}

		ImGui::Spacing();

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

	if (m_lmbPressed == true)
	{
		XMFLOAT2 clickPosWorld = GameManager::GetInstance()->GetCamera()->GetMousePositionInWorldSpace();

		Tile* ptile;

		int row = -1;
		int column = -1;

		if (m_ptileMap->GetTileFromWorldPos(clickPosWorld, ptile, &row, &column) == false)
		{
			return;
		}

		if (ptile == nullptr && m_toolMode != ToolMode::DELETE_TILE)
		{
			m_ptileMap->CreateTile(row, column, ptile);
		}

		switch (m_toolMode)
		{
		case ToolMode::DELETE_TILE:
			if (ptile != nullptr)
			{
				m_ptileMap->DeleteTile(row, column);

				ptile = nullptr;
			}
			break;

		case ToolMode::LAYER:
			ptile->SetLayer(m_paintLayer);
			break;

		case ToolMode::SELECT:
			m_selectedTile = DirectX::XMINT2(row, column);
			break;

		case ToolMode::TEXTURE:
			if (m_ppaintSRV != nullptr)
			{
				ptile->SetAlbedo(m_ppaintSRV);
			}
			break;

		default:
			break;
		}
	}
}

void TileMapTool::Destroy()
{
	ToolBase::Destroy();

	EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonPressed, this);
	EventManager::Instance()->RemoveClientEvent(EventType::MouseButtonReleased, this);
}

void TileMapTool::Handle(Event* e)
{
	if (m_selectingDimensions == true)
	{
		return;
	}

	switch (e->GetEventID())
	{
	case EventType::MouseButtonPressed:
		MouseButtonPressed((MouseButtonPressedEvent*)e);
		break;

	case EventType::MouseButtonReleased:
		MouseButtonReleased((MouseButtonReleasedEvent*)e);
		break;

	default:
		break;
	}
}

void TileMapTool::MouseButtonPressed(MouseButtonPressedEvent* e)
{
	if (e->GetButton() == VK_LBUTTON)
	{
		m_lmbPressed = true;
	}
}

void TileMapTool::MouseButtonReleased(MouseButtonReleasedEvent* e)
{
	if (e->GetButton() == VK_LBUTTON)
	{
		m_lmbPressed = false;
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
		m_ptileMap = GameManager::GetInstance()->CreateGameObject<TileMap>("TileMap");
		m_ptileMap->Init(m_tileMapWidth, m_tileMapHeight, XMFLOAT3(0, 0, 0), m_tileDimensions);

		m_selectingDimensions = false;
	}

	ImGui::End();
}

#endif