#include "EditorUI.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Includes/IMGUI/imgui_internal.h"
#include <ShlObj_core.h>

#if EDITOR
HWND* EditorUI::m_phwnd = nullptr;

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

void EditorUI::DrawEditorUI(ID3D11Device* pdevice)
{
	DrawSceneGraphWindow();

	DrawSceneManagementWindow();

	if (GameManager::GetInstance()->GetSelectedGameObject() != nullptr)
	{
		GameManager::GetInstance()->GetSelectedGameObject()->ShowEngineUI();
	}

	m_contentBrowser.Draw();

}

void EditorUI::Update()
{
	ImGuiIO& io = ImGui::GetIO();

	io.DisplaySize = ImVec2(GraphicsManager::GetInstance()->GetWindowDimensions().x, GraphicsManager::GetInstance()->GetWindowDimensions().y);

	io.DeltaTime = GameManager::GetInstance()->GetTimer()->DeltaTime();
}

void EditorUI::DrawSceneGraphWindow()
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

				ImGui::EndMenu();
			}



			if (ImGui::MenuItem("ParticleSystem"))
			{

			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Delete"))
		{
			if (ImGui::MenuItem("GameObject"))
			{
				pgameManager->DeleteSelectedGameObject();

				m_gameObjectNodeClicked = -1;
				pgameManager->SelectGameObject(nullptr);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
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
				pgameManager->CreateGameObject<RenderableCollidableGameObject>(gameObjectName);
				break;

			case GameObjectType::RENDERABLE:
				pgameManager->CreateGameObject<RenderableGameObject>(gameObjectName);
				break;

			case GameObjectType::COLLIDABLE:
				pgameManager->CreateGameObject<CollidableGameObject>(gameObjectName);
				break;

			case GameObjectType::BASE:
				pgameManager->CreateGameObject<GameObject>(gameObjectName);
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
				OpenFileExplorer(L"DDS files\0*.dds\0", m_texNameBuffer, _countof(m_texNameBuffer));
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

	bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)nodeCount, node_flags, pcurrentNode->GameObject->GetIdentifier().c_str(), nodeCount);
	
	if (ImGui::BeginDragDropSource() == true)
	{
		bool test = ImGui::SetDragDropPayload("SceneGraph", &pcurrentNode->GameObject, sizeof(pcurrentNode->GameObject), ImGuiCond_Once);
		ImGui::EndDragDropSource();
	}
	
	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	{
		if (nodeCount == m_gameObjectNodeClicked)
		{
			m_gameObjectNodeClicked = -1;
			pgameManager->SelectGameObject(nullptr);


		}
		else
		{
			m_gameObjectNodeClicked = nodeCount;
			pgameManager->SelectGameObjectUsingTreeNode(pcurrentNode);
		}
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

void EditorUI::DragFloat2(const string& label, XMFLOAT2& values, const float& columnWidth, const float& speed, const float& min, const float& max)
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = io.FontDefault->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::Button("X", buttonSize);
	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, speed, min, max, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::Button("Y", buttonSize);
	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, speed, min, max, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

void EditorUI::DragFloat3(const string& label, XMFLOAT3& values, const float& columnWidth, const float& speed, const float& min, const float& max)
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = io.FontDefault->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::Button("X", buttonSize);
	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, speed, min, max, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::Button("Y", buttonSize);
	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, speed, min, max, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::Button("Z", buttonSize);
	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, speed, min, max, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

ImGui::PopID();
}

void EditorUI::DragInt(const string& label, int& value, const float& columnWidth, const float& speed, const float& min, const float& max)
{
	ImGui::PushID(label.c_str());

	ImGui::Columns(2);

	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushItemWidth(ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	ImGui::DragInt("##Int", &value, speed, min, max, "%.2f");

	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

void EditorUI::Checkbox(const string& label, bool& value, const float& columnWidth)
{
	ImGui::PushID(label.c_str());

	ImGui::Columns(2);

	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
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
		const ImGuiPayload* ppayload = ImGui::AcceptDragDropPayload("ContentBrowser", ImGuiDragDropFlags_SourceAllowNullID);

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

bool EditorUI::InputText(const string& label, string& text, const float& columnWidth)
{
	bool interacted = false;

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);

	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
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

void EditorUI::IdentifierText(const string& label, string& text, const float& columnWidth)
{
	ImGui::PushID(label.c_str());

	ImGui::Columns(2);

	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
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
			if (GraphicsManager::GetInstance()->LoadAnimationFromFile(filepath) == false)
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

bool EditorUI::DragFloat(const string& label, float& value, const float& columnWidth, const float& speed, const float& min, const float& max)
{
	bool interacted = false;

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);

	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushItemWidth(ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	interacted = ImGui::DragFloat("##Float", &value, speed, min, max, "%.2f");

	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();

	return interacted;
}

#endif
