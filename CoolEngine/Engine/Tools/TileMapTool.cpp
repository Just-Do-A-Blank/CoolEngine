#include "TileMapTool.h"

#include "Engine/Includes/IMGUI/imgui.h"
#include "Engine/Includes/IMGUI/imgui_impl_win32.h"
#include "Engine/Includes/IMGUI/imgui_impl_dx11.h"

void TileMapTool::DrawTileMapManager()
{
	
}

void TileMapTool::Render()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//DrawTileMapManager();

	ImGui::Begin("Tile Map Manager", nullptr, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Create Tile Map", "Ctrl+C"))
			{
				m_createTileMapClicked = true;

			}

			if (ImGui::MenuItem("Edit Tile Map", "Ctrl+E"))
			{

			}

			if (ImGui::MenuItem("Save Tile Map", "Ctrl+S"))
			{
				// Do stuff
			}

			if (ImGui::MenuItem("Open Tile Map", "Ctrl+O"))
			{
				//OpenFileExplorer(L"DDS files\0*.dds\0", m_texNameBuffer, _countof(m_texNameBuffer));
			}

			if (ImGui::MenuItem("Delete Tile Map", "Ctrl+D"))
			{

			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();

	}

	static int selected = -1;
	int mapCount = 0;
	for (unordered_map<string, TileMap>::iterator it = tileMapList.begin(); it != tileMapList.end(); ++it)
	{
		string sceneName = it->first;
		if (ImGui::Selectable(sceneName.c_str(), selected == mapCount))
		{
			if (selected != mapCount)
			{
				selected = mapCount;
				//pgameManager->SelectScene(it->second);
			}

			else
			{
				selected = -1;
				//pgameManager->SelectScene(nullptr);
			}
		}
		++mapCount;
	}

	if (m_createTileMapClicked)
	{
		ImGui::Begin("New Tile Map");
		static char tileMapName[64] = "";
		static char emptyChar[64] = "";

		IMGUI_LEFT_LABEL(ImGui::InputText, "Tile Map Name", tileMapName, 64);

		int clicked = 0;

		if (ImGui::Button("Create"))
		{
			++clicked;
		}

		if (ImGui::Button("Cancel"))
		{
			m_createTileMapClicked = false;
		}

		if (clicked & 1)
		{
			newMap = TileMap();
			tileMapList[tileMapName] = newMap;
			tileMapName[0] = ' ';
			m_createTileMapClicked = false;
		}

		ImGui::End();
	}

	ImGui::End();



	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGui::EndFrame();
}
