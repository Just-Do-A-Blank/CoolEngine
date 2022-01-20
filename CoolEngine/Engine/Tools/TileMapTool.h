#pragma once
#include "ToolBase.h"

#include "Engine/Includes/IMGUI/imgui.h"
#include "Engine/Includes/IMGUI/imgui_impl_win32.h"
#include "Engine/Includes/IMGUI/imgui_impl_dx11.h"
#include "Engine/ResourceDefines.h"

#include "Engine/TileMap/TileMap/TileMap.h"

#define IMGUI_LEFT_LABEL(func, label, ...) (ImGui::TextUnformatted(label), ImGui::SameLine(), func("##" label, __VA_ARGS__))
#define FILEPATH_BUFFER_SIZE 200
#define DEFAULT_IMGUI_IMAGE_SIZE ImVec2(256, 256)

class TileMapTool : public ToolBase
{
public:

	bool m_createTileMapClicked;

	void DrawTileMapManager();

protected:
	virtual void Render() override;

private:
	TileMap newMap;

	unordered_map<string, TileMap> tileMapList;
};

