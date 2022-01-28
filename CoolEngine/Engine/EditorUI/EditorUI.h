#pragma once
#include "Engine/Includes/IMGUI/imgui.h"
#include "Engine/Includes/IMGUI/imgui_impl_win32.h"
#include "Engine/Includes/IMGUI/imgui_impl_dx11.h"
#include "Engine/ResourceDefines.h"
#include "Engine/Graphics/SpriteAnimation.h"
#include "Engine/Managers/SceneGraph.h"

#define IMGUI_LEFT_LABEL(func, label, ...) (ImGui::TextUnformatted(label), ImGui::SameLine(), func("##" label, __VA_ARGS__))
#define DEFAULT_IMGUI_IMAGE_SIZE ImVec2(256, 256)

#if EDITOR

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
	static HWND* m_phwnd;
	bool m_createSceneClicked = false;
	bool m_createGameObjectClicked = false;
	bool m_deleteGameObjectClicked = false;

	//Master window
	bool g_ShowSceneEditor;
	bool g_ShowSceneManagement;
	bool g_ShowGameObject;

	int num = 1;

	ImGuiTreeNodeFlags m_base_flags;
	int m_selectionMask;
	int m_gameObjectNodeClicked = -1;

	WCHAR m_texNameBuffer[FILEPATH_BUFFER_SIZE];

	int m_animNameUpdateIndex = -1;
	string m_animUpdateName = "";

	void DrawMasterWindow();
	void DrawSceneGraphWindow();
	void DrawSceneManagementWindow();
	void TraverseTree(TreeNode<GameObject>* pcurrentNode, int& count);
public:
	EditorUI(ID3D11Device* pdevice);

	void InitIMGUI(ID3D11DeviceContext* pcontext, ID3D11Device* pdevice, HWND* hwnd);
	void ShutdownIMGUI();
	void DrawEditorUI(ID3D11Device* pdevice);

	void Update();

	static void OpenFileExplorer(const WCHAR* fileFilters, WCHAR* buffer, int bufferSize);
	static void OpenFolderExplorer(WCHAR* buffer, int bufferSize);

	static bool DragFloat(const string& label, float& value, const float& columnWidth = 100.0f, const float& speed = 0.1f, const float& min = 0, const float& max = 0);
	static void DragFloat2(const string& label, XMFLOAT2& values, const float& columnWidth = 100.0f, const float& speed = 0.1f, const float& min = 0, const float& max = 0);
	static void DragFloat3(const string& label, XMFLOAT3& values, const float& columnWidth = 100.0f, const float& speed = 0.1f, const float& min = 0, const float& max = 0);

	static void DragInt(const string& label, int& value, const float& columnWidth = 100.0f, const float& speed = 0.1f, const float& min = 0, const float& max = 0);

	static void Checkbox(const string& label, bool& value, const float& columnWidth = 100.0f);

	static bool InputText(const string& label, string& text, const float& columnWidth = 100.0f);
	static void IdentifierText(const string& label, string& text, const float& columnWidth = 100.0f);

	static bool Texture(const string& label, wstring& filepath, ID3D11ShaderResourceView*& psrv, const float& columnWidth = 100.0f);
	static bool Animation(const string& label, wstring& filepath, ID3D11ShaderResourceView* psrv, const float& columnWidth = 100.0f);

	static void Animations(const string& label, unordered_map<string, SpriteAnimation>& animations, const float& columnWidth = 100.0f);
};

#endif