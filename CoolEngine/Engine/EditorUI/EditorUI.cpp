#include "EditorUI.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/GameObjects/GameObject.h"

#include <ShlObj_core.h>
#include "Engine/Managers/SceneGraph.h"

void EditorUI::InitIMGUI(ID3D11DeviceContext* pcontext, ID3D11Device* pdevice, HWND* phwnd)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.WantCaptureMouse = true;

	(void)io;

	ImGui::StyleColorsDark();

	m_phwnd = phwnd;

	ImGui_ImplWin32_Init(*m_phwnd);
	ImGui_ImplDX11_Init(pdevice, pcontext);
}

EditorUI::EditorUI(ID3D11Device* pdevice)
{
	m_pdevice = pdevice;
}

void EditorUI::DrawEditorUI()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	DrawMasterWindow();

	if (g_ShowSceneEditor)
	{
		DrawSceneGraphWindow();
	}

	if (g_ShowSceneManagement)
	{
		DrawSceneManagementWindow();
	}

	if (g_ShowGameObject)
	{
		DrawGameObjectPropertiesWindow();
	}

	ImGui::Render();

	m_deleteGameObjectClicked = false;

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void EditorUI::Update()
{
	for (int i = 0; i < m_perAnimation.size(); ++i)
	{
		m_perAnimation[i].m_spriteAnim.Update();
	}
}

void EditorUI::DrawMasterWindow()
{
	ImGui::Begin("Master Window");
	ImGui::Checkbox("Scene Graph Window", &g_ShowSceneEditor);
	ImGui::Checkbox("Scene Management Window", &g_ShowSceneManagement);
	ImGui::Checkbox("GameObject Properties Window", &g_ShowGameObject);
	ImGui::End();
}

void EditorUI::DrawSceneGraphWindow()
{
	if (!m_pselectedScene)
	{
		return;
	}
	ImGui::Begin("Scene Graph", nullptr, ImGuiWindowFlags_MenuBar);

	GameManager* pgameManager = GameManager::GetInstance();
	static int selected = -1;

	TreeNode* prootNode = m_pselectedScene->GetRootTreeNode();

	m_base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	int nodeCount = -1;

	if (!m_deleteGameObjectClicked)
	{
		TraverseTree(prootNode, nodeCount);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("GameObject"))
			{
				m_createGameObjectClicked = true;
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
				m_deleteGameObjectClicked = true;
				m_pselectedScene->DeleteGameObjectUsingNode(m_pselectedGameObjectNode);

				m_gameObjectNodeClicked = -1;
				m_pselectedGameObjectNode = nullptr;
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	ImGui::End();

	if (m_createGameObjectClicked)
	{
		ImGui::Begin("New GameObject");
		static char gameObjectName[64] = "";
		IMGUI_LEFT_LABEL(ImGui::InputText, "GameObject Name", gameObjectName, 64);

		int clicked = 0;
		if (ImGui::Button("Create"))
		{
			++clicked;
		}
		if (clicked & 1)
		{
			if (!m_rootGameObject || !m_pselectedGameObjectNode)
			{
				//CreateRootGameObject
				m_rootGameObject = m_pselectedScene->CreateGameObject(gameObjectName);
			}
			else
			{
				//Create child gameObject
				m_pselectedScene->CreateGameObject(gameObjectName, m_pselectedGameObjectNode);
			}
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
				m_pselectedScene = it->second;
			}

			else
			{
				selected = -1;
				m_pselectedScene = nullptr;
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

void EditorUI::OnGameObjectSelected()
{
	m_perAnimation.clear();

	std::unordered_map<std::string, SpriteAnimation>* panimations = m_pselectedGameObject->GetAnimations();

	m_perAnimation.reserve(panimations->size());

	int count = 0;

	for (std::unordered_map<std::string, SpriteAnimation>::iterator it = panimations->begin(); it != panimations->end(); ++it)
	{
		memcpy(m_perAnimation[count].m_animName, it->first.c_str(), it->first.length());

		m_perAnimation[count].m_spriteAnim = it->second;
	}
}

float pos[3] =
{
	10, 1, 0
};

void EditorUI::DrawGameObjectPropertiesWindow()
{
	ImGui::Begin("GameObject Properties");

	ImGui::Separator();
	ImGui::Spacing();

	IMGUI_LEFT_LABEL(ImGui::DragFloat3, "Position", pos);
	IMGUI_LEFT_LABEL(ImGui::DragFloat3, "Rotation", pos);
	IMGUI_LEFT_LABEL(ImGui::DragFloat3, "Scale", pos);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::TextUnformatted("Texture");

	if (ImGui::ImageButton((void*)(intptr_t)GraphicsManager::GetInstance()->GetShaderResourceView(m_texNameBuffer), DEFAULT_IMGUI_IMAGE_SIZE))
	{
		WCHAR tempNameBuffer[FILEPATH_BUFFER_SIZE];

		memcpy(&tempNameBuffer, &m_texNameBuffer, FILEPATH_BUFFER_SIZE);

		OpenFileExplorer(L"DDS files\0*.dds\0", m_texNameBuffer, _countof(m_texNameBuffer));

		wstring relativePath = m_texNameBuffer;

		if (relativePath == L"")
		{
			memcpy(&m_texNameBuffer, &tempNameBuffer, FILEPATH_BUFFER_SIZE);
		}
		else
		{
			int index = relativePath.find(L"Resources");

			if (index == -1)
			{
				LOG("The resource specified isn't stored in a resource folder!");
			}
			else
			{
				relativePath = wstring(m_texNameBuffer).substr(index);

				relativePath.copy(m_texNameBuffer, relativePath.size());

				m_texNameBuffer[relativePath.size()] = L'\0';
			}
		}

	}

	ImGui::Spacing();

	IMGUI_LEFT_LABEL(ImGui::Checkbox, "Renderable", &g_ShowSceneEditor);
	IMGUI_LEFT_LABEL(ImGui::Checkbox, "Collidable", &g_ShowSceneEditor);
	IMGUI_LEFT_LABEL(ImGui::Checkbox, "Trigger", &g_ShowSceneEditor);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	ImGui::TextUnformatted("Animation");

	std::unordered_map<std::string, SpriteAnimation>* panimations = m_pselectedGameObject->GetAnimations();

	int count = 0;

	for (std::unordered_map<std::string, SpriteAnimation>::iterator it = panimations->begin(); it != panimations->end(); ++it)
	{
		if (ImGui::InputText("Name", m_perAnimation[count].m_animName, ANIM_NAME_SIZE) == true)
		{
			if (m_pselectedGameObject->GetAnimations()->count(m_perAnimation[count].m_animName) == 0)
			{
				m_animNameUpdateIndex = count;
				m_animUpdateName = it->first;
			}
			else
			{
				LOG("Tried to add an animation with the same local name as one that already exists!");
			}
		}

		if (ImGui::ImageButton((void*)(intptr_t)it->second.GetCurrentFrame(), DEFAULT_IMGUI_IMAGE_SIZE))
		{
			WCHAR tempNameBuffer[FILEPATH_BUFFER_SIZE];

			memcpy(&tempNameBuffer, &m_animFilepath, FILEPATH_BUFFER_SIZE);

			OpenFolderExplorer(m_animFilepath, _countof(m_animFilepath));

			wstring relativePath = m_animFilepath;

			if (GraphicsManager::GetInstance()->GetAnimation(m_animFilepath).GetFrames() == nullptr)
			{
				if (GraphicsManager::GetInstance()->LoadAnimationFromFile(m_animFilepath, m_pdevice) == false)
				{
					LOG("Failed to load the animation!");
				}
				else
				{
					m_perAnimation[count].m_spriteAnim = GraphicsManager::GetInstance()->GetAnimation(m_animFilepath);

					m_pselectedGameObject->OverwriteAnimation(it->first, m_perAnimation[count].m_spriteAnim);
				}
			}
		}

		++count;
	}

	if (m_animNameUpdateIndex != -1)
	{
		SpriteAnimation tempAnim = panimations->at(m_animUpdateName);

		panimations->erase(m_animUpdateName);

		panimations->insert(pair<string, SpriteAnimation>(string(m_perAnimation[m_animNameUpdateIndex].m_animName), tempAnim));

		m_animNameUpdateIndex = -1;
	}

	ImGui::End();
}

void EditorUI::TraverseTree(TreeNode* pcurrentNode, int& nodeCount)
{
	if (!pcurrentNode)
	{
		return;
	}

	++nodeCount;
	ImGuiTreeNodeFlags node_flags = m_base_flags;
	const bool is_selected = (m_selectionMask & (1 << nodeCount)) != 0;


	if (is_selected)
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)nodeCount, node_flags, pcurrentNode->GameObject->GetIdentifier().c_str(), nodeCount);
	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	{
		if (nodeCount == m_gameObjectNodeClicked)
		{
			m_gameObjectNodeClicked = -1;
			m_pselectedGameObjectNode = nullptr;

		}
		else
		{
			m_gameObjectNodeClicked = nodeCount;
			m_pselectedGameObjectNode = pcurrentNode;
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
