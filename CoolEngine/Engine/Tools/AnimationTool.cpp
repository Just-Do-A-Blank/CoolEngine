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
}

void AnimationTool::Update()
{
}

void AnimationTool::Render()
{
	bool updateAnim = false;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Animation");

	for (int i = 0; i < m_frameInfos.size(); ++i)
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

			if (updateAnim == false)
			{
				updateAnim = EditorUI::Texture("Texture", m_frameInfos[i].m_filepath, m_frameInfos[i].m_frame.m_ptexture);
			}
			else
			{
				EditorUI::Texture("Texture", m_frameInfos[i].m_filepath, m_frameInfos[i].m_frame.m_ptexture);
			}

			ImGui::Spacing();

			if (updateAnim == false)
			{
				updateAnim = EditorUI::DragFloat("Time (s)", m_frameInfos[i].m_frame.m_frameTime);
			}
			else
			{
				EditorUI::DragFloat("Time (s)", m_frameInfos[i].m_frame.m_frameTime);
			}


			ImGui::TreePop();
		}
	}

	ImGui::Spacing();

	if (ImGui::Button("New") == true)
	{
		m_frameInfos.push_back(FrameInfo());
	}

	ImGui::SameLine();

	if (ImGui::Button("Delete") == true && m_selectedIndex != -1)
	{
		m_frameInfos.erase(m_frameInfos.begin() + m_selectedIndex);

		m_selectedIndex = -1;
	}

	ImGui::SameLine();

	if (ImGui::Button("Save") == true)
	{
		WCHAR savePath[FILEPATH_BUFFER_SIZE];
		EditorUI::OpenFolderExplorer(savePath, FILEPATH_BUFFER_SIZE);

		m_savePath = savePath;

		if (m_savePath != L"")	//If path specified then create folder
		{

		}
	}

	ImGui::End();

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGui::EndFrame();

	if (updateAnim)
	{
		SpriteAnimation anim;

		m_frames.clear();

		for (int i = 0; i < m_frameInfos.size(); ++i)
		{
			m_frames.push_back(m_frameInfos[i].m_frame);
		}

		anim.SetFrames(&m_frames);

		m_pgameObject->RemoveAnimation("Anim");

		m_pgameObject->AddAnimation("Anim", anim);
	}
}

void AnimationTool::Destroy()
{
}
