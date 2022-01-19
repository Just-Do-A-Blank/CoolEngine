#pragma once
#include "Engine/Includes/IMGUI/imgui.h"
#include "Engine/Includes/IMGUI/imgui_impl_win32.h"
#include "Engine/Includes/IMGUI/imgui_impl_dx11.h"
#include "Engine/Scene/Scene.h"

#define IMGUI_LEFT_LABEL(func, label, ...) (ImGui::TextUnformatted(label), ImGui::SameLine(), func("##" label, __VA_ARGS__))
#define FILEPATH_BUFFER_SIZE 200
#define ANIM_NAME_SIZE 50
#define DEFAULT_IMGUI_IMAGE L"Resources\\Sprites\\Brick.dds"
#define DEFAULT_IMGUI_ANIMATION L"TestAnim"
#define DEFAULT_IMGUI_IMAGE_SIZE ImVec2(256, 256)

class GameManager;
class GameObject;

struct SelectableText
{
	int index;
	string identifier;
};

struct PerAnimation
{
	char m_animName[ANIM_NAME_SIZE];

	SpriteAnimation m_spriteAnim;
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
	WCHAR m_animFilepath[FILEPATH_BUFFER_SIZE] = DEFAULT_IMGUI_ANIMATION;

	vector<PerAnimation> m_perAnimation;

	int m_animNameUpdateIndex = -1;
	string m_animUpdateName = "";

	GameObject* m_pselectedGameObject;

	void OnGameObjectSelected();

	void OpenFileExplorer(const WCHAR* fileFilters, WCHAR* buffer, int bufferSize);
	void OpenFolderExplorer(WCHAR* buffer, int bufferSize);

	void DrawMasterWindow();
	void DrawSceneGraphWindow();
	void DrawSceneManagementWindow();
	void DrawGameObjectPropertiesWindow();

	static ImGuiInputTextCallback AnimNameCallback();

	ID3D11Device* m_pdevice = nullptr;

	void TraverseTree(TreeNode* pcurrentNode, int& count);
public:
	EditorUI(ID3D11Device* pdevice);

	void InitIMGUI(ID3D11DeviceContext* pcontext, ID3D11Device* pdevice, HWND* hwnd);
	void ShutdownIMGUI();
	void DrawEditorUI();

	void Update();
};
