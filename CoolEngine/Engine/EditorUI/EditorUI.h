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
class GameObject;

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
	static HWND* m_phwnd;
	bool m_createSceneClicked = false;
	bool m_createGameObjectClicked = false;
	bool m_deleteGameObjectClicked = false;

	bool m_createTileMapClicked = false;


	//Master window
	bool g_ShowSceneEditor;
	bool g_ShowSceneManagement;
	bool g_ShowGameObject;
	bool g_ShowTileMapManager;

	int num = 1;
	Scene* m_pselectedScene = nullptr;
	TreeNode* m_pselectedGameObjectNode = nullptr;
	GameObject* m_rootGameObject = nullptr;

	ImGuiTreeNodeFlags m_base_flags;
	int m_selectionMask;
	int m_gameObjectNodeClicked = -1;

	WCHAR m_texNameBuffer[FILEPATH_BUFFER_SIZE];

	int m_animNameUpdateIndex = -1;
	string m_animUpdateName = "";

	GameObject* m_pselectedGameObject;

	void DrawMasterWindow();
	void DrawSceneGraphWindow();
	void DrawSceneManagementWindow();

	void DrawTileMapManager();

	ID3D11Device* m_pdevice = nullptr;

	void TraverseTree(TreeNode* pcurrentNode, int& count);
public:
	EditorUI(ID3D11Device* pdevice);

	void InitIMGUI(ID3D11DeviceContext* pcontext, ID3D11Device* pdevice, HWND* hwnd);
	void ShutdownIMGUI();
	void DrawEditorUI(ID3D11Device* pdevice);

	static void OpenFileExplorer(const WCHAR* fileFilters, WCHAR* buffer, int bufferSize);
	static void OpenFolderExplorer(WCHAR* buffer, int bufferSize);
};
