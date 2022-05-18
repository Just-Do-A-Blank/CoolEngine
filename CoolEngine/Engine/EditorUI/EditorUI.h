#pragma once
#include "Engine/Includes/IMGUI/imgui.h"
#include "Engine/Includes/IMGUI/imgui_impl_win32.h"
#include "Engine/Includes/IMGUI/imgui_impl_dx11.h"
#include "Engine/ResourceDefines.h"
#include "Engine/Graphics/SpriteAnimation.h"
#include "Engine/Managers/SceneGraph.h"
#include "Engine/EditorUI/ContentBrowser.h"
#include "Engine/GameUI/GameUIComponent.h"
#include <Engine\EditorUI\EditorUIFloatParameters.h>
#include <Engine\EditorUI\EditorUIIntParameters.h>
#include <Engine\EditorUI\EditorUINonSpecificParameters.h>

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
	bool m_createUIObjectClicked = false;
	bool m_deleteGameObjectClicked = false;
	static bool s_bisViewportHovered;

	static DirectX::XMFLOAT2 s_viewportSize;
	static DirectX::XMFLOAT2 s_viewportPosition;

	int num = 1;

	ImGuiTreeNodeFlags m_base_flags;
	int m_selectionMask;
	int m_gameObjectNodeClicked = -1;
	TreeNode<GameObject>* m_selectedGameObjectNode;

	WCHAR m_texNameBuffer[FILEPATH_BUFFER_SIZE];

	int m_animNameUpdateIndex = -1;
	string m_animUpdateName = "";

	GameObjectType m_createObjectType;
	UIComponentType m_createUIComponentType;
	ContentBrowser m_contentBrowser;

	ID3D11ShaderResourceView* m_playButtonTexture;
	ID3D11ShaderResourceView* m_stopButtonTexture;


	void DrawSceneGraphWindow(ToolBase*& ptoolBase, ID3D11Device* pdevice);
	void DrawSceneManagementWindow();
	void TraverseTree(TreeNode<GameObject>* pcurrentNode, int& count);
public:
	EditorUI(ID3D11Device* pdevice);

	void InitIMGUI(ID3D11DeviceContext* pcontext, ID3D11Device* pdevice, HWND* hwnd);
	void ShutdownIMGUI();
	void DrawEditorUI(ID3D11Device* pdevice, ToolBase*& ptoolBase);

	void DrawPlayButtonWindow(XMFLOAT2 buttonSize, float verticalOffset);

	static void SetIsViewportHovered(bool bHovered);
	static bool GetIsViewportHovered();

	static void SetViewportSize(DirectX::XMFLOAT2 size);
	static DirectX::XMFLOAT2 GetViewportSize();

	static void SetViewportPosition(DirectX::XMFLOAT2 pos);
	static DirectX::XMFLOAT2 GetViewportPosition();

	void Update();

	static void OpenFileExplorer(const WCHAR* fileFilters, WCHAR* buffer, int bufferSize);
	static void OpenFolderExplorer(WCHAR* buffer, int bufferSize);

    /// <summary>
    /// A title which spans the full length of the component editor window
    /// </summary>
    /// <param name="label">The title to display</param>
    /// <param name="columnWidth">The width of a single coloumn</param>
	static void FullTitle(const string& label, EditorUINonSpecificParameters parameters = {});

	static void ToolTip(const char* desc);

	/// <summary>
	/// Creates a section of UI which is sectioned off. Major sections of the UI should be in collapsing sections
	/// such that they do not clutter the User Interface
	/// </summary>
	/// <param name="label">The label to use in the UI</param>
	/// <param name="defaultValue">true means open on load, will retain value between equal headers.</param>
	/// <returns>Use this in the if statement, returns if the area is open or closed</returns>
	static bool CollapsingSection(const string& label, bool defaultValue = true);

    static bool DragFloat(const string& label, float& value, EditorUIFloatParameters parameters = {});
	static void DragFloat2(const string& label, XMFLOAT2& values1f, EditorUIFloatParameters parameters = {});
	static void DragFloat3(const string& label, XMFLOAT3& values, EditorUIFloatParameters parameters = {});

    static void DragInt(const string& label, int& value, EditorUIIntParameters parameters = {});

    static void Checkbox(const string& label, bool& value, EditorUINonSpecificParameters parameters = {});

	static bool InputText(const string& label, string& text, EditorUINonSpecificParameters parameters = {});
	static void IdentifierText(const string& label, string& text, EditorUINonSpecificParameters parameters = {});

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

private:

    /// <summary>
    /// Default width of a coloumn
    /// </summary>
    /// <returns>Default width of coloumns</returns>
    static float GetDefaultColumnWidth() { return 100; }

    /// <summary>
    /// Default movement speed with sliders
    /// </summary>
    /// <returns>Default speed</returns>
    static float GetDefaultSpeed() { return 0.1f; }

    /// <summary>
    /// Default minimum value of a number entry value
    /// </summary>
    /// <returns>Default minimum</returns>
    static float GetDefaultMinimumValue() { return 0; }

    /// <summary>
    /// Default maximum value of a number entry value
    /// </summary>
    /// <returns>Default maximum</returns>
    static float GetDefaultMaximumValue() { return 0; }

    /// <summary>
    /// Sets up the default parameters for floats with defaults where optional parameters were opt-out
    /// </summary>
    /// <param name="parameters">Parameters optionally required to display a float or set of floats in the editor</param>
    static void SetupDefaultsInParameters(EditorUIFloatParameters& parameters);

    /// <summary>
    /// Sets up the default parameters for ints with defaults where optional parameters were opt-out
    /// </summary>
    /// <param name="parameters">Parameters optionally required to display a ints or set of ints in the editor</param>
    static void SetupDefaultsInParameters(EditorUIIntParameters& parameters);

    /// <summary>
    /// Sets up the default parameters for inputs which do not have any data which relate to one another
    /// </summary>
    /// <param name="parameters">Sets up the default parameters for inputs which do not have any data which relate to one another</param>
    static void SetupDefaultsInParameters(EditorUINonSpecificParameters& parameters);

    /// <summary>
    /// If tooltip text found, it will add tooltip text to which ever element is just displayed from IMGUI
    /// </summary>
    /// <param name="tooltipText">Text to display in the tooltip</param>
    static void SetupTooltip(char* tooltipText);
};

#endif