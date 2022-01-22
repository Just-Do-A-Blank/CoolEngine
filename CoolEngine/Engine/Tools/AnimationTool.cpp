#include "AnimationTool.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/ResourceDefines.h"

void AnimationTool::Init(ID3D11Device* pdevice)
{
	ToolBase::Init(pdevice);

	XMFLOAT3 pos = XMFLOAT3(0, 0, 0);
	XMFLOAT3 scale = XMFLOAT3(100, 100, 1);

	m_pgameObject = GameManager::GetInstance()->CreateGameObject<GameObject>("AnimModel");
	m_pgameObject->GetTransform()->SetPosition(pos);
	m_pgameObject->GetTransform()->SetScale(scale);
	m_pgameObject->SetAlbedo(DEFAULT_IMGUI_IMAGE);
}

void AnimationTool::Update()
{
}

void AnimationTool::Render()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Animation");

	for (int i = 0; i < m_frames.size(); ++i)
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (m_selectedIndex == i)
		{
			node_flags |= ImGuiTreeNodeFlags_Selected;
		}

		if (ImGui::TreeNodeEx((string("Frame ") + to_string(i)).c_str(), node_flags) == true)
		{
			if (ImGui::IsItemClicked())
			{
				if (m_selectedIndex == i)
				{
					m_selectedIndex = -1;
				}
				else
				{
					m_selectedIndex = i;
				}
			}

			if (ImGui::ImageButton((void*)(intptr_t)m_frames[i].m_frame.m_ptexture, DEFAULT_IMGUI_IMAGE_SIZE))
			{
				EditorUI::OpenFileExplorer(L"DDS files\0*.dds\0", m_frames[i].m_texName, _countof(m_frames[i].m_texName));

				wstring relativePath = m_frames[i].m_texName;

				//Check if that path points to an asset in the resources folder
				int index = relativePath.find(L"Resources");

				if (index == -1)
				{
					LOG("The resource specified isn't stored in a resource folder!");
				}
				else
				{
					//Get relative file path
					relativePath = wstring(m_frames[i].m_texName).substr(index);

					relativePath.copy(m_frames[i].m_texName, relativePath.size());

					m_frames[i].m_texName[relativePath.size()] = L'\0';

					//Load texture if not loaded
					if (GraphicsManager::GetInstance()->IsTextureLoaded(m_frames[i].m_texName) == true)
					{
						GraphicsManager::GetInstance()->LoadTextureFromFile(m_frames[i].m_texName);
					}

					m_frames[i].m_frame.m_ptexture = GraphicsManager::GetInstance()->GetShaderResourceView(m_frames[i].m_texName);
				}
			}

			ImGui::DragFloat("Time (s)", &m_frames[i].m_frame.m_frameTime, 0.1f, 0.0f);

			ImGui::TreePop();
		}
	}

	if (ImGui::Button("New") == true)
	{
		m_frames.push_back(FrameInfo());
	}

	ImGui::SameLine();

	if (ImGui::Button("Delete") == true && m_selectedIndex != -1)
	{
		m_frames.erase(m_frames.begin() + m_selectedIndex);

		m_selectedIndex = -1;
	}

	ImGui::SameLine();

	if (ImGui::Button("Save") == true)
	{
		EditorUI::OpenFolderExplorer(m_savePath, _countof(m_savePath));

		wstring relativePath = m_savePath;

		if (relativePath != L"")
		{

		}
	}

	ImGui::End();

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGui::EndFrame();
}

void AnimationTool::Destroy()
{
}
