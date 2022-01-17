#include "EditorUI.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/GameObjects/GameObject.h"

#include <ShlObj_core.h>

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
	ImGui::Begin("Scene Graph", nullptr, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("GameObject"))
			{

			}

			if (ImGui::MenuItem("ParticleSystem"))
			{

			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	ImGui::End();
}

void EditorUI::DrawSceneManagementWindow()
{
	GameManager* pgameManager = GameManager::GetInstance();

	ImGui::Begin("Scene Manager", nullptr, ImGuiWindowFlags_MenuBar);
	bool flag = false;
	if (ImGui::TreeNode("SceneList"))
	{
		static int selected = -1;
		unordered_map<string, Scene*> sceneList = pgameManager->GetSceneList();
		int sceneCount = 0;
		ImGui::Indent();
		for (unordered_map<string, Scene*>::iterator it = sceneList.begin(); it != sceneList.end(); ++it)
		{
			string sceneName = it->first;
			if (ImGui::Selectable(sceneName.c_str(), selected == sceneCount))
			{
				selectedScene = it->second;
				selected = sceneCount;
			}
			++sceneCount;
		}
		ImGui::TreePop();
		if (selectedScene)
		{
			LOG(selectedScene->GetSceneName());
		}
	}
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Create Scene", "Ctrl+C"))
			{
				string name = "Test" + to_string(num);
				pgameManager->CreateScene(name);
				++num;
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
