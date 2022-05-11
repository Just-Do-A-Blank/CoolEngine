#pragma once
#include "Engine/Includes/IMGUI/imgui.h"
#include "Engine/Includes/IMGUI/imgui_impl_win32.h"
#include "Engine/Includes/IMGUI/imgui_impl_dx11.h"
#include "Engine/ResourceDefines.h"
#include "Engine/Graphics/SpriteAnimation.h"
#include "Engine/Managers/SceneGraph.h"
#include "Engine/EditorUI/ContentBrowser.h"

#define IMGUI_LEFT_LABEL(func, label, ...) (ImGui::TextUnformatted(label), ImGui::SameLine(), func("##" label, __VA_ARGS__))
#define DEFAULT_IMGUI_IMAGE_SIZE ImVec2(256, 256)

#if EDITOR

class ToolBase;
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
	static bool s_bisViewportHovered;

	static DirectX::XMFLOAT2 s_viewportSize;
	static DirectX::XMFLOAT2 s_viewportPosition;

	int num = 1;

	ImGuiTreeNodeFlags m_base_flags;
	int m_selectionMask;
	int m_gameObjectNodeClicked = -1;
	TreeNode<GameObject>* m_selecedGameObjectNode;

	WCHAR m_texNameBuffer[FILEPATH_BUFFER_SIZE];

	int m_animNameUpdateIndex = -1;
	string m_animUpdateName = "";

	GameObjectType m_createObjectType;
	ContentBrowser m_contentBrowser;


	void DrawSceneGraphWindow(ToolBase*& ptoolBase, ID3D11Device* pdevice);
	void DrawSceneManagementWindow();
	void TraverseTree(TreeNode<GameObject>* pcurrentNode, int& count);
public:
	EditorUI(ID3D11Device* pdevice);

	void InitIMGUI(ID3D11DeviceContext* pcontext, ID3D11Device* pdevice, HWND* hwnd);
	void ShutdownIMGUI();
	void DrawEditorUI(ID3D11Device* pdevice, ToolBase*& ptoolBase);

	static void SetIsViewportHovered(bool bHovered);
	static bool GetIsViewportHovered();

	static void SetViewportSize(DirectX::XMFLOAT2 size);
	static DirectX::XMFLOAT2 GetViewportSize();

	static void SetViewportPosition(DirectX::XMFLOAT2 pos);
	static DirectX::XMFLOAT2 GetViewportPosition();

	void Update();

	static void OpenFileExplorer(const WCHAR* fileFilters, WCHAR* buffer, int bufferSize);
	static void OpenFolderExplorer(WCHAR* buffer, int bufferSize);

	static bool DragFloat(const string& label, float& value, const float& columnWidth = 100.0f, const float& speed = 0.1f, const float& min = 0, const float& max = 0);
	static void DragFloat2(const string& label, XMFLOAT2& values, const float& columnWidth = 100.0f, const float& speed = 0.1f, const float& min = 0, const float& max = 0);
	static void DragFloat3(const string& label, XMFLOAT3& values, const float& columnWidth = 100.0f, const float& speed = 0.1f, const float& min = 0, const float& max = 0);

	static void DragInt(const string& label, int& value, const float& columnWidth = 100.0f, const float& speed = 0.1f, const int& min = 0, const int& max = 0);

	static void Checkbox(const string& label, bool& value, const float& columnWidth = 100.0f);

	static bool InputText(const string& label, string& text, const float& columnWidth = 100.0f);
	static void IdentifierText(const string& label, string& text, const float& columnWidth = 100.0f);

	static bool Texture(const string& label, wstring& filepath, ID3D11ShaderResourceView*& psrv, const float& columnWidth = 100.0f, ImVec2& imageDimensions = DEFAULT_IMGUI_IMAGE_SIZE);
	static bool Animation(const string& label, wstring& filepath, ID3D11ShaderResourceView* psrv, const float& columnWidth = 100.0f);

	static void Animations(const string& label, unordered_map<string, SpriteAnimation>& animations, const float& columnWidth = 100.0f);

	template<class T>
	static void ReferenceField(const string& label, T*& objectPointer, const float& columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushItemWidth(ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });


		char buffer[FILEPATH_BUFFER_SIZE];

		if (objectPointer == nullptr)
		{
			strcpy_s(buffer, "Null");
		}
		else
		{
			GameObject* pobject = (GameObject*)objectPointer;

			strcpy_s(buffer, pobject->GetIdentifier().c_str());
		}

		ImGui::Text(buffer);

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* ppayload = ImGui::AcceptDragDropPayload("SceneGraphNode");

			if (ppayload != nullptr)
			{
				objectPointer = (*((TreeNode<T>**)ppayload->Data))->NodeObject;
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}
};

#endif