#if EDITOR

#include "TileMapTool.h"

#include "Engine/Includes/IMGUI/imgui.h"
#include "Engine/Includes/IMGUI/imgui_impl_win32.h"
#include "Engine/Includes/IMGUI/imgui_impl_dx11.h"
#include "Engine/EditorUI/EditorUI.h"
#include "Engine/Tools/TileMapTool.h"
#include "Engine/TileMap/TileMap/TileMap.h"

void TileMapTool::Init(ID3D11Device* pdevice)
{
	ToolBase::Init(pdevice);
}

void TileMapTool::Render()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (m_selectingDimensions == true)
	{
		CreateSelectDimensionsUI();
	}
	else
	{
		ImGui::Begin("Properties");

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::End();
	}

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGui::EndFrame();
}

void TileMapTool::Update()
{
	if (m_selectingDimensions == true)
	{
		return;
	}
}

void TileMapTool::CreateSelectDimensionsUI()
{
	ImGui::Begin("Tile Map Properties");

	EditorUI::DragInt("Width", m_tileMapWidth);
	EditorUI::DragInt("Height", m_tileMapHeight);

	if (ImGui::Button("Create") == true)
	{
		m_ptileMap = new TileMap(m_tileMapWidth, m_tileMapHeight, "TileMap", XMFLOAT3(0, 0, 0));

		m_selectingDimensions = false;
	}

	ImGui::End();
}

#endif