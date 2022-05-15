#include "EditorUI.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Includes/IMGUI/imgui_internal.h"
#include "Engine/Physics/ParticleSystem.h"
#include "Engine/GameObjects/InteractableGameObject.h"
#include "Engine/GameObjects/WeaponGameObject.h"
#include "Engine\GameObjects\EnemyGameObject.h"
#include "Engine/Tools/ToolBase.h"
#include "Engine/Tools/AnimationTool.h"
#include "Engine/Tools/InGameUITool.h"
#include "Engine//Tools/TileMapTool.h"

#include <ShlObj_core.h>

#if EDITOR
HWND* EditorUI::m_phwnd = nullptr;
bool EditorUI::s_bisViewportHovered = false;
DirectX::XMFLOAT2 EditorUI::s_viewportSize = DirectX::XMFLOAT2(0, 0);
DirectX::XMFLOAT2 EditorUI::s_viewportPosition = DirectX::XMFLOAT2(0, 0);

void EditorUI::InitIMGUI(ID3D11DeviceContext* pcontext, ID3D11Device* pdevice, HWND* phwnd)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.WantCaptureMouse = true;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	float fontSize = 18.0f;
	io.FontDefault = io.Fonts->AddFontFromFileTTF("Resources/UI/Fonts/OpenSans-Regular.ttf", fontSize);

	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	m_phwnd = phwnd;

	ImGui_ImplWin32_Init(*m_phwnd);
	ImGui_ImplDX11_Init(pdevice, pcontext);
}

EditorUI::EditorUI(ID3D11Device* pdevice)
{
}

void EditorUI::DrawEditorUI(ID3D11Device* pdevice, ToolBase*& ptoolBase)
{
	DrawSceneGraphWindow(ptoolBase, pdevice);

	DrawSceneManagementWindow();

	if (m_selecedGameObjectNode != nullptr)
	{
		m_selecedGameObjectNode->NodeObject->ShowEngineUI();
	}

	m_contentBrowser.Draw();

}

void EditorUI::SetIsViewportHovered(bool bHovered)
{
	s_bisViewportHovered = bHovered;
}

bool EditorUI::GetIsViewportHovered()
{
	return s_bisViewportHovered;
}

void EditorUI::Update()
{
	ImGuiIO& io = ImGui::GetIO();

	io.DisplaySize = ImVec2(GraphicsManager::GetInstance()->GetWindowDimensions().x, GraphicsManager::GetInstance()->GetWindowDimensions().y);

	io.DeltaTime = GameManager::GetInstance()->GetTimer()->DeltaTime();
}

void EditorUI::SetViewportSize(DirectX::XMFLOAT2 size)
{
	s_viewportSize = size;
}

DirectX::XMFLOAT2 EditorUI::GetViewportSize()
{
	return s_viewportSize;
}

void EditorUI::SetViewportPosition(DirectX::XMFLOAT2 position)
{
	s_viewportPosition = position;
}

DirectX::XMFLOAT2 EditorUI::GetViewportPosition()
{
	return s_viewportPosition;
}

void EditorUI::DrawSceneGraphWindow(ToolBase*& ptoolBase, ID3D11Device* pdevice)
{
	if (!GameManager::GetInstance()->GetCurrentScene())
	{
		return;
	}
	ImGui::Begin("Scene Graph", nullptr, ImGuiWindowFlags_MenuBar);
	
	GameManager* pgameManager = GameManager::GetInstance();
	static int selected = -1;

	TreeNode<GameObject>* prootNode = pgameManager->GetRootTreeNode();

	m_base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	int nodeCount = -1;

	TraverseTree(prootNode, nodeCount);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::BeginMenu("GameObjects"))
			{
				if (ImGui::MenuItem("Base"))
				{
					m_createGameObjectClicked = true;

					m_createObjectType = GameObjectType::BASE;
				}
				if (ImGui::MenuItem("Collidable"))
				{
					m_createGameObjectClicked = true;

					m_createObjectType = GameObjectType::COLLIDABLE;
				}
				if (ImGui::MenuItem("Renderable"))
				{
					m_createGameObjectClicked = true;

					m_createObjectType = GameObjectType::RENDERABLE;
				}
				if (ImGui::MenuItem("Renderable Collidable"))
				{
					m_createGameObjectClicked = true;

					m_createObjectType = GameObjectType::RENDERABLE | GameObjectType::COLLIDABLE;
				}
				if (ImGui::MenuItem("Player"))
				{
					m_createGameObjectClicked = true;

					m_createObjectType = GameObjectType::PLAYER;
				}
				if (ImGui::MenuItem("Enemy"))
				{
					m_createGameObjectClicked = true;

					m_createObjectType = GameObjectType::ENEMY;
				}
				if (ImGui::MenuItem("Interactable"))
				{
					m_createGameObjectClicked = true;

					m_createObjectType = GameObjectType::INTERACTABLE;
				}
				if (ImGui::MenuItem("Weapon"))
				{
					m_createGameObjectClicked = true;

					m_createObjectType = GameObjectType::WEAPON;
				}

				ImGui::EndMenu();
			}



			if (ImGui::MenuItem("ParticleSystem"))
			{
				m_createGameObjectClicked = true;

				m_createObjectType = GameObjectType::PARTICLESYSTEM;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Delete"))
		{
			if (ImGui::MenuItem("GameObject"))
			{
				pgameManager->DeleteGameObjectUsingNode(m_selecedGameObjectNode);

				m_gameObjectNodeClicked = -1;
				m_selecedGameObjectNode = nullptr;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("Animation"))
			{
				if (ptoolBase != nullptr)
				{
					delete ptoolBase;
				}

				ptoolBase = new AnimationTool();
				ptoolBase->Init(pdevice);
			}

			if (ImGui::MenuItem("In Game UI"))
			{
				if (ptoolBase != nullptr)
				{
					delete ptoolBase;
				}

				ptoolBase = new InGameUITool();
				ptoolBase->Init(pdevice);
			}

			if (ImGui::MenuItem("Tile Map"))
			{
				if (ptoolBase != nullptr)
				{
					delete ptoolBase;
				}

				ptoolBase = new TileMapTool();
				ptoolBase->Init(pdevice);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::BeginChild("Root Drag & Drop Region");
	ImGui::EndChild();

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* ppayload = ImGui::AcceptDragDropPayload("SceneGraphNode", ImGuiDragDropFlags_None);

		if (ppayload != nullptr)
		{
			TreeNode<GameObject>* objectPointer = *(TreeNode<GameObject>**)ppayload->Data;
			if (objectPointer->PreviousParent != nullptr)
			{
				pgameManager->GetCurrentScene()->GetSceneGraph()->MoveNode(objectPointer, nullptr);
			}
		}

		ImGui::EndDragDropTarget();
	}

	if (ImGui::IsItemClicked())
	{
		m_gameObjectNodeClicked = -1;
		m_selecedGameObjectNode = nullptr;
	}
	
	ImGui::End();

	if (m_createGameObjectClicked)
	{
		ImGui::Begin("New Object");
		static char gameObjectName[64] = "";
		IMGUI_LEFT_LABEL(ImGui::InputText, "Object Name", gameObjectName, 64);

		int clicked = 0;
		if (ImGui::Button("Create"))
		{
			++clicked;
		}
		if (clicked & 1)
		{
			switch (m_createObjectType)
			{
			case GameObjectType::RENDERABLE | GameObjectType::COLLIDABLE:
				pgameManager->CreateGameObject<RenderableCollidableGameObject>(gameObjectName, m_selecedGameObjectNode);
				break;

			case GameObjectType::RENDERABLE:
				pgameManager->CreateGameObject<RenderableGameObject>(gameObjectName, m_selecedGameObjectNode);
				break;

			case GameObjectType::COLLIDABLE:
				pgameManager->CreateGameObject<CollidableGameObject>(gameObjectName, m_selecedGameObjectNode);
				break;

			case GameObjectType::BASE:
				pgameManager->CreateGameObject<GameObject>(gameObjectName, m_selecedGameObjectNode);
				break;

			case GameObjectType::PARTICLESYSTEM:
				pgameManager->CreateGameObject<ParticleSystem>(gameObjectName, m_selecedGameObjectNode);
				break;

			case GameObjectType::PLAYER:
				pgameManager->CreateGameObject<PlayerGameObject>(gameObjectName, m_selecedGameObjectNode);
				break;

            case GameObjectType::ENEMY:
                pgameManager->CreateGameObject<EnemyGameObject>(gameObjectName, m_selecedGameObjectNode);
                break;

			case GameObjectType::INTERACTABLE:
				pgameManager->CreateGameObject<InteractableGameObject>(gameObjectName, m_selecedGameObjectNode);
				break;

			case GameObjectType::WEAPON:
				pgameManager->CreateGameObject<WeaponGameObject>(gameObjectName, m_selecedGameObjectNode);
				break;
			}


			m_createObjectType =(GameObjectType) 0;

			m_createGameObjectClicked = false;
			gameObjectName[0] = {};
		}
		
		ImGui::End();
	}
}

void EditorUI::DrawSceneManagementWindow()
{
	GameManager* pgameManager = GameManager::GetInstance();

	ImGui::Begin("Scene Manager", nullptr, ImGuiWindowFlags_MenuBar);
	bool flag = false;

	static int selected = -1;
	unordered_map<string, Scene*> sceneList = pgameManager->GetSceneList();
	int sceneCount = 0;
	for (unordered_map<string, Scene*>::iterator it = sceneList.begin(); it != sceneList.end(); ++it)
	{
		string sceneName = it->first;
		if (ImGui::Selectable(sceneName.c_str(), selected == sceneCount))
		{
			if (selected != sceneCount)
			{
				selected = sceneCount;
				pgameManager->SelectScene(it->second);
			}

			else
			{
				selected = -1;
				pgameManager->SelectScene(nullptr);
			}
		}
		++sceneCount;
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Create Scene", "Ctrl+C"))
			{
				m_createSceneClicked = true;
			}

			if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
			{
				/* Do stuff */
			}

			if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
			{
				OpenFileExplorer(L"Scene files\0*.json\0", m_texNameBuffer, _countof(m_texNameBuffer));
			}

			if (ImGui::MenuItem("Delete Scene", "Ctrl+D"))
			{
				pgameManager->DeleteSelectedScene();
				selected = -1;
				pgameManager->SelectScene(nullptr);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::End();

	if (m_createSceneClicked)
	{
		ImGui::Begin("New Scene");
		static char sceneName[64] = "";
		IMGUI_LEFT_LABEL(ImGui::InputText, "Scene Name", sceneName, 64);

		int clicked = 0;

		if (ImGui::Button("Create"))
		{
			++clicked;
		}

		if (clicked & 1)
		{
			pgameManager->CreateScene(sceneName);
			m_createSceneClicked = false;
			sceneName[0] = {};
		}
		ImGui::End();
	}
}

void EditorUI::TraverseTree(TreeNode<GameObject>* pcurrentNode, int& nodeCount)
{
	if (!pcurrentNode)
	{
		return;
	}

	GameManager* pgameManager = GameManager::GetInstance();

	++nodeCount;
	ImGuiTreeNodeFlags node_flags = m_base_flags;
	const bool is_selected = (m_selectionMask & (1 << nodeCount)) != 0;


	if (is_selected)
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)nodeCount, node_flags, pcurrentNode->NodeObject->GetIdentifier().c_str(), nodeCount);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* ppayload = ImGui::AcceptDragDropPayload("SceneGraphNode", ImGuiDragDropFlags_None);

		if (ppayload != nullptr)
		{
			TreeNode<GameObject>* objectPointer = *(TreeNode<GameObject>**)ppayload->Data;

			pgameManager->GetCurrentScene()->GetSceneGraph()->MoveNode(objectPointer, pcurrentNode);
		}

		ImGui::EndDragDropTarget();
	}

	if (ImGui::BeginDragDropSource() == true)
	{
		bool test = ImGui::SetDragDropPayload("SceneGraphNode", &pcurrentNode, sizeof(pcurrentNode), ImGuiCond_Once);
		ImGui::EndDragDropSource();
	}

	if (ImGui::IsMouseReleased(0) && ImGui::IsItemHovered())
	{
		m_gameObjectNodeClicked = nodeCount;
		m_selecedGameObjectNode = pcurrentNode;
	}

	if (node_open)
	{
		if (pcurrentNode->Child)
		{
			TraverseTree(pcurrentNode->Child, nodeCount);
		}
		ImGui::TreePop();
	}

	if (pcurrentNode->Sibling)
	{
		TraverseTree(pcurrentNode->Sibling, nodeCount);
	}


	if (m_gameObjectNodeClicked != -1)
	{
		m_selectionMask = (1 << m_gameObjectNodeClicked); // Click to single-select
	}
	else
	{
		//Resets selection
		m_selectionMask = 0;
	}

}

void EditorUI::ShutdownIMGUI()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void EditorUI::OpenFileExplorer(const WCHAR* fileFilters, WCHAR* buffer, int bufferSize)
{
	OPENFILENAME ofn;

	//Null terminate first index so no information from buffer is displayed
	buffer[0] = '\0';

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = *m_phwnd;
	ofn.lpstrFile = buffer;
	ofn.nMaxFile = bufferSize;
	ofn.lpstrFilter = fileFilters;
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	GetOpenFileName(&ofn);
}

void EditorUI::OpenFolderExplorer(WCHAR* buffer, int bufferSize)
{
	buffer[0] = '\0';

	BROWSEINFOW browserInfo;
	ZeroMemory(&browserInfo, sizeof(BROWSEINFOW));
	browserInfo.hwndOwner = *m_phwnd;
	browserInfo.pidlRoot = NULL;
	browserInfo.pszDisplayName = buffer;
	browserInfo.lpszTitle = L"Choose animation file";
	browserInfo.ulFlags = BIF_EDITBOX;
	browserInfo.lpfn = NULL;
	browserInfo.lParam = NULL;
	browserInfo.iImage = 0;

	SHBrowseForFolder(&browserInfo);
}

void EditorUI::DragFloat2(const string& label, XMFLOAT2& values, EditorUIFloatParameters parameters)
{
    SetupDefaultsInParameters(parameters);

	ImGuiIO& io = ImGui::GetIO();

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, parameters.m_columnWidth);
	ImGui::Text(label.c_str());
    SetupTooltip(parameters.m_tooltipText);

	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = io.FontDefault->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::Button("X", buttonSize);
	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, parameters.m_speed, parameters.m_minValue, parameters.m_maxValue, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::Button("Y", buttonSize);
	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, parameters.m_speed, parameters.m_minValue, parameters.m_maxValue, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

void EditorUI::DragFloat3(const string& label, XMFLOAT3& values, EditorUIFloatParameters parameters)
{
    SetupDefaultsInParameters(parameters);

	ImGuiIO& io = ImGui::GetIO();

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, parameters.m_maxValue);
	ImGui::Text(label.c_str());
    SetupTooltip(parameters.m_tooltipText);

	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = io.FontDefault->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::Button("X", buttonSize);
	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, parameters.m_speed, parameters.m_minValue, parameters.m_maxValue, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::Button("Y", buttonSize);
	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, parameters.m_speed, parameters.m_minValue, parameters.m_maxValue, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::Button("Z", buttonSize);
	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, parameters.m_speed, parameters.m_minValue, parameters.m_maxValue, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

ImGui::PopID();
}

void EditorUI::DragInt(const string& label, int& value, EditorUIIntParameters parameters)
{
    SetupDefaultsInParameters(parameters);

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);

	ImGui::SetColumnWidth(0, parameters.m_columnWidth);
	ImGui::Text(label.c_str());
    SetupTooltip(parameters.m_tooltipText);

	ImGui::NextColumn();

	ImGui::PushItemWidth(ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	ImGui::DragInt("##Int", &value, parameters.m_speed, parameters.m_minValue, parameters.m_maxValue, "%.2f");

	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

void EditorUI::Checkbox(const string& label, bool& value, EditorUINonSpecificParameters parameters)
{
    SetupDefaultsInParameters(parameters);

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);

	ImGui::SetColumnWidth(0, parameters.m_columnWidth);
	ImGui::Text(label.c_str());
    SetupTooltip(parameters.m_tooltipText);

	ImGui::NextColumn();

	ImGui::PushItemWidth(ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	ImGui::Checkbox("##Checkbox", &value);

	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

bool EditorUI::Texture(const string& label, wstring& filepath, ID3D11ShaderResourceView*& psrv, const float& columnWidth, ImVec2& imageDimensions)
{
	bool interacted = false;

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);

	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());

	ImGui::NextColumn();

	ImGui::PushItemWidth(ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	if (ImGui::ImageButton((void*)(intptr_t)psrv, imageDimensions))
	{
		WCHAR buffer[FILEPATH_BUFFER_SIZE];

		EditorUI::OpenFileExplorer(L"DDS files\0*.dds\0", buffer, FILEPATH_BUFFER_SIZE);

		filepath = wstring(buffer);

		//Check if that path points to an asset in the resources folder
		int index = filepath.find(L"Resources");

		if (index == -1)
		{
			LOG("The resource specified isn't stored in a resource folder!");
		}
		else
		{
			//Get relative file path
			filepath = filepath.substr(index);

			//Load texture if not loaded
			if (GraphicsManager::GetInstance()->IsTextureLoaded(filepath) == false)
			{
				GraphicsManager::GetInstance()->LoadTextureFromFile(filepath);
			}

			psrv = GraphicsManager::GetInstance()->GetShaderResourceView(filepath);

			interacted = true;
		}
	}

	if(ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* ppayload = ImGui::AcceptDragDropPayload("ContentBrowserFile", ImGuiDragDropFlags_SourceAllowNullID);

		if (ppayload != nullptr)
		{
			std::string tempString = std::string((char*)ppayload->Data, ppayload->DataSize / sizeof(char));
			std::wstring wsfilepath = std::wstring(tempString.begin(), tempString.end());

			//Check if that path points to an asset in the resources folder
			int index = wsfilepath.find(L"Resources");

			if (index == -1)
			{
				LOG("The resource specified isn't stored in a resource folder!");
			}
			else
			{
				//Get relative file path
				wsfilepath = wsfilepath.substr(index);

				//Load texture if not loaded
				bool bloaded = true;

				if (GraphicsManager::GetInstance()->IsTextureLoaded(wsfilepath) == false)
				{
					bloaded = GraphicsManager::GetInstance()->LoadTextureFromFile(wsfilepath);
				}

				if (bloaded == true)
				{
					psrv = GraphicsManager::GetInstance()->GetShaderResourceView(wsfilepath);
				}

				interacted = true;
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();

	return interacted;
}

bool EditorUI::InputText(const string& label, string& text, EditorUINonSpecificParameters parameters)
{
    SetupDefaultsInParameters(parameters);

	bool interacted = false;

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);

	ImGui::SetColumnWidth(0, parameters.m_columnWidth);
	ImGui::Text(label.c_str());
    SetupTooltip(parameters.m_tooltipText);

	ImGui::NextColumn();

	ImGui::PushItemWidth(ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });


	char buffer[FILEPATH_BUFFER_SIZE];

	strcpy_s(buffer, text.c_str());

	if (ImGui::InputText("##text", buffer, FILEPATH_BUFFER_SIZE))
	{
		interacted = true;
	}

	text = string(buffer);

	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();

	return interacted;
}

void EditorUI::IdentifierText(const string& label, string& text, EditorUINonSpecificParameters parameters)
{
    SetupDefaultsInParameters(parameters);

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);

	ImGui::SetColumnWidth(0, parameters.m_columnWidth);
	ImGui::Text(label.c_str());
    SetupTooltip(parameters.m_tooltipText);

	ImGui::NextColumn();

	ImGui::PushItemWidth(ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	ImGui::TextUnformatted(text.c_str());

	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

bool EditorUI::Animation(const string& label, wstring& filepath, ID3D11ShaderResourceView* psrv, const float& columnWidth)
{
	bool interacted = false;

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);

	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());

	ImGui::NextColumn();

	ImGui::PushItemWidth(ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	if (ImGui::TreeNode(label.c_str()) == true)
	{
		//Animation images
		if (ImGui::ImageButton((void*)(intptr_t)psrv, DEFAULT_IMGUI_IMAGE_SIZE) == true)
		{
			WCHAR buffer[FILEPATH_BUFFER_SIZE];

			EditorUI::OpenFolderExplorer(buffer, FILEPATH_BUFFER_SIZE);

			if (buffer[0] != '\0')
			{
				filepath = wstring(buffer);

				interacted = true;
			}
		}

		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* ppayload = ImGui::AcceptDragDropPayload("ContentBrowserDirectory", ImGuiDragDropFlags_SourceAllowNullID);

			if (ppayload != nullptr)
			{
				std::string tempString = std::string((char*)ppayload->Data, ppayload->DataSize / sizeof(char));
				filepath = wstring(tempString.begin(), tempString.end());

				interacted = true;
			}

			ImGui::EndDragDropTarget();
		}

		ImGui::TreePop();
	}

	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();

	return interacted;
}

void EditorUI::Animations(const string& label, unordered_map<string, SpriteAnimation>& animations, const float& columnWidth)
{
	char animName[ANIM_NAME_SIZE];

	wstring filepath = L"";

	string animOldName = "";
	string animNewName = "";

	bool updateAnim = false;
	bool updateAnimName = false;

	for (unordered_map<string, SpriteAnimation>::iterator it = animations.begin(); it != animations.end(); ++it)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);

		strcpy_s(animName, it->first.c_str());

		if (ImGui::InputText("##Name", animName, ANIM_NAME_SIZE) == true)
		{
			animOldName = it->first;
			animNewName = string(animName);

			updateAnimName = animNewName != "";
		}

		ImGui::NextColumn();

		ImGui::PushItemWidth(ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		if (ImGui::TreeNode(label.c_str()) == true)
		{
			//Animation images
			if (ImGui::ImageButton((void*)(intptr_t)it->second.GetCurrentFrame(), DEFAULT_IMGUI_IMAGE_SIZE) == true)
			{
				WCHAR buffer[FILEPATH_BUFFER_SIZE];

				EditorUI::OpenFolderExplorer(buffer, FILEPATH_BUFFER_SIZE);

				filepath = wstring(buffer);

				animOldName = it->first;

				updateAnim = filepath != L"";
			}

			if (ImGui::BeginDragDropTarget())
			{
				const ImGuiPayload* ppayload = ImGui::AcceptDragDropPayload("ContentBrowserDirectory", ImGuiDragDropFlags_SourceAllowNullID);

				if (ppayload != nullptr)
				{
					std::string tempString = std::string((char*)ppayload->Data, ppayload->DataSize / sizeof(char));
					filepath = wstring(tempString.begin(), tempString.end());

					updateAnim = true;
				}

				ImGui::EndDragDropTarget();
			}

			ImGui::TreePop();
		}

		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	if (updateAnimName == true)
	{
		SpriteAnimation tempAnim = animations[animOldName];

		animations.erase(animOldName);

		animations.insert(pair<string, SpriteAnimation>(animNewName, tempAnim));
	}

	if (updateAnim == true)
	{
		SpriteAnimation anim = GraphicsManager::GetInstance()->GetAnimation(filepath);

		if (anim.GetFrames() == nullptr)
		{
			if (GraphicsManager::GetInstance()->LoadAnimationFromFile(filepath) == true)
			{
				animations[animName] = GraphicsManager::GetInstance()->GetAnimation(filepath);
			}
			else
			{
				LOG("Failed to load the animation!");
			}
		}
		else
		{
			animations[animName] = anim;
		}
	}
}

bool EditorUI::DragFloat(const string& label, float& value, EditorUIFloatParameters parameters)
{
    SetupDefaultsInParameters(parameters);

	bool interacted = false;

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);

	ImGui::SetColumnWidth(0, parameters.m_columnWidth);
	ImGui::Text(label.c_str());
    SetupTooltip(parameters.m_tooltipText);

	ImGui::NextColumn();

	ImGui::PushItemWidth(ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	interacted = ImGui::DragFloat("##Float", &value, parameters.m_speed, parameters.m_minValue, parameters.m_maxValue, "%.2f");

	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();

	return interacted;
}

/// <summary>
/// A title which spans the full length of the component editor window
/// </summary>
/// <param name="label">The title to display</param>
/// <param name="columnWidth">The width of a single coloumn</param>
void EditorUI::FullTitle(const string& label, EditorUINonSpecificParameters parameters)
{
    SetupDefaultsInParameters(parameters);

    ImGui::Separator();
    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, parameters.m_columnWidth);
    ImGui::NextColumn();

    ImGui::SetColumnWidth(0, parameters.m_columnWidth);
    ImGui::Text(label.c_str());
    SetupTooltip(parameters.m_tooltipText);

    ImGui::NextColumn();

    ImGui::PushItemWidth(ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
    
    ImGui::PopItemWidth();
    ImGui::PopStyleVar();
    ImGui::PopID();

    ImGui::Separator();
}

/// <summary>
/// Sets up the default parameters for floats with defaults where optional parameters were opt-out
/// </summary>
/// <param name="parameters">Parameters optionally required to display a float or set of floats in the editor</param>
void EditorUI::SetupDefaultsInParameters(EditorUIFloatParameters& parameters)
{
    if (parameters.m_columnWidth == NULL)
    {
        parameters.m_columnWidth = GetDefaultColumnWidth();
    }

    if (parameters.m_speed == NULL)
    {
        parameters.m_speed = GetDefaultSpeed();
    }

    if (parameters.m_minValue == NULL)
    {
        parameters.m_minValue = GetDefaultMinimumValue();
    }

    if (parameters.m_maxValue == NULL)
    {
        parameters.m_maxValue = GetDefaultMaximumValue();
    }

    if (parameters.m_tooltipText == NULL)
    {
        parameters.m_tooltipText = "";
    }
}

/// <summary>
/// Sets up the default parameters for ints with defaults where optional parameters were opt-out
/// </summary>
/// <param name="parameters">Parameters optionally required to display a ints or set of ints in the editor</param>
void EditorUI::SetupDefaultsInParameters(EditorUIIntParameters& parameters)
{
    if (parameters.m_columnWidth == NULL)
    {
        parameters.m_columnWidth = GetDefaultColumnWidth();
    }

    if (parameters.m_speed == NULL)
    {
        parameters.m_speed = GetDefaultSpeed();
    }

    if (parameters.m_minValue == NULL)
    {
        parameters.m_minValue = (int)GetDefaultMinimumValue();
    }

    if (parameters.m_maxValue == NULL)
    {
        parameters.m_maxValue = (int)GetDefaultMaximumValue();
    }

    if (parameters.m_tooltipText == NULL)
    {
        parameters.m_tooltipText = "";
    }
}

/// <summary>
/// Sets up the default parameters for inputs which do not have any data which relate to one another
/// </summary>
/// <param name="parameters">Sets up the default parameters for inputs which do not have any data which relate to one another</param>
void EditorUI::SetupDefaultsInParameters(EditorUINonSpecificParameters& parameters)
{
    if (parameters.m_columnWidth == NULL)
    {
        parameters.m_columnWidth = GetDefaultColumnWidth();
    }

    if (parameters.m_tooltipText == NULL)
    {
        parameters.m_tooltipText = "";
    }
}

/// <summary>
/// If tooltip text found, it will add tooltip text to which ever element is just displayed from IMGUI
/// </summary>
/// <param name="tooltipText">Text to display in the tooltip</param>
void EditorUI::SetupTooltip(char* tooltipText)
{
    if (tooltipText == "")
    {
        return;
    }

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(tooltipText);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}
#endif
