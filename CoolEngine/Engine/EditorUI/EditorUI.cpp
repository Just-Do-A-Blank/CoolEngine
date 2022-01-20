#include "EditorUI.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/Managers/SceneGraph.h"
#include "Engine/Managers/GraphicsManager.h"

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


	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGui::EndFrame();
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
	if (!GameManager::GetInstance()->GetCurrentScene())
	{
		return;
	}
	ImGui::Begin("Scene Graph", nullptr, ImGuiWindowFlags_MenuBar);

	GameManager* pgameManager = GameManager::GetInstance();
	static int selected = -1;
	
	TreeNode* prootNode = pgameManager->GetRootTreeNode();

	m_base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	int nodeCount = -1;

	TraverseTree(prootNode, nodeCount);

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
			pgameManager->CreateGameObject<GameObject>(gameObjectName);
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

float pos[3] =
{
	10, 1, 0
};

void EditorUI::DrawGameObjectPropertiesWindow()
{
	ImGui::Begin("GameObject Properties");

	ImGui::Separator();
	ImGui::Spacing();

	//IMGUI_LEFT_LABEL(ImGui::DragFloat3, "Position", g_ptestObject->GetTransform()->GetPositionRef().);
	IMGUI_LEFT_LABEL(ImGui::DragFloat3, "Position", pos);
	IMGUI_LEFT_LABEL(ImGui::DragFloat3, "Rotation", pos);
	IMGUI_LEFT_LABEL(ImGui::DragFloat3, "Scale", pos);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
	char buf[100];

	if (ImGui::ImageButton((void*)(intptr_t)GraphicsManager::GetInstance()->GetShaderResourceView(m_texNameBuffer), DEFAULT_IMGUI_IMAGE_SIZE))
	{
		OpenFileExplorer(L"DDS files\0*.dds\0", m_texNameBuffer, _countof(m_texNameBuffer));

		wstring relativePath = m_texNameBuffer;

		int index = relativePath.find(L"Resources");

		if (index == relativePath.npos)
		{
			LOG("The resource specified isn't stored in a resource folder!");
		}

		relativePath = wstring(m_texNameBuffer).substr(index);

		relativePath.copy(m_texNameBuffer, relativePath.size());

		m_texNameBuffer[relativePath.size()] = L'\0';
	}

	IMGUI_LEFT_LABEL(ImGui::InputText, "Animation Name", buf, IM_ARRAYSIZE(buf));

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	IMGUI_LEFT_LABEL(ImGui::Checkbox, "Renderable", &g_ShowSceneEditor);
	IMGUI_LEFT_LABEL(ImGui::Checkbox, "Collidable", &g_ShowSceneEditor);
	IMGUI_LEFT_LABEL(ImGui::Checkbox, "Trigger", &g_ShowSceneEditor);

	ImGui::End();
}

void EditorUI::TraverseTree(TreeNode* pcurrentNode, int& nodeCount)
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
