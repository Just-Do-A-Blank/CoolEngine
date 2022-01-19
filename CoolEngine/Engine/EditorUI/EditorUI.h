#pragma once
#include "Engine/Includes/IMGUI/imgui.h"
#include "Engine/Includes/IMGUI/imgui_impl_win32.h"
#include "Engine/Includes/IMGUI/imgui_impl_dx11.h"
#include "Engine/Scene/Scene.h"
#include "Engine/ResourceDefines.h"

#define IMGUI_LEFT_LABEL(func, label, ...) (ImGui::TextUnformatted(label), ImGui::SameLine(), func("##" label, __VA_ARGS__))
#define FILEPATH_BUFFER_SIZE 200
#define DEFAULT_IMGUI_IMAGE_SIZE ImVec2(256, 256)

class GameManager;

struct SelectableText
{
	int index;
	string identifier;
};

enum class SceneManagementState
{
	SCENE_WINDOW,
	CREATE_SCENE_WINDOW
};


class EditorUI
{
private:
	HWND* m_phwnd;
	bool m_createSceneClicked = false;
	bool m_createGameObjectClicked = false;
	bool m_deleteGameObjectClicked = false;

	//Master window
	bool g_ShowSceneEditor;
	bool g_ShowSceneManagement;
	bool g_ShowGameObject;

	int num = 1;
	Scene* m_pselectedScene = nullptr;
	TreeNode* m_pselectedGameObjectNode = nullptr;
	GameObject* m_rootGameObject = nullptr;

	ImGuiTreeNodeFlags m_base_flags;
	int m_selectionMask;
	int m_gameObjectNodeClicked = -1;

	//Gameobject properties
	WCHAR m_texNameBuffer[FILEPATH_BUFFER_SIZE] = DEFAULT_IMGUI_IMAGE;

	void OpenFileExplorer(const WCHAR* fileFilters, WCHAR* buffer, int bufferSize);

	void DrawMasterWindow();
	void DrawSceneGraphWindow();
	void DrawSceneManagementWindow();
	void DrawGameObjectPropertiesWindow();
	void TraverseTree(TreeNode* pcurrentNode, int& count);
public:
	void InitIMGUI(ID3D11DeviceContext* pcontext, ID3D11Device* pdevice, HWND* hwnd);
	void ShutdownIMGUI();
	void DrawEditorUI();


};

