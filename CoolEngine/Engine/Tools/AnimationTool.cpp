#if EDITOR

#include "AnimationTool.h"
#include "Engine/Managers/GameManager.h"
#include "Engine/ResourceDefines.h"
#include "Engine/GameObjects/RenderableGameObject.h"

#include <direct.h>
#include <fstream>

void AnimationTool::Init(ID3D11Device* pdevice)
{
	ToolBase::Init(pdevice);

	XMFLOAT3 pos = XMFLOAT3(0, 0, 0);
	XMFLOAT3 scale = XMFLOAT3(100, 100, 1);

	m_pgameObject = GameManager::GetInstance()->CreateGameObject<RenderableGameObject>("AnimModel");
	m_pgameObject->GetTransform()->SetPosition(pos);
	m_pgameObject->GetTransform()->SetScale(scale);
}

void AnimationTool::Update()
{
	m_pgameObject->Update();
}

void AnimationTool::Render()
{
	bool updateAnim = false;

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
				updateAnim = EditorUI::Texture("Texture", m_frameInfos[i].m_filepath, m_frameInfos[i].m_frame.m_ptexture, 100.0f, ImVec2(50, 50));
			}
			else
			{
				EditorUI::Texture("Texture", m_frameInfos[i].m_filepath, m_frameInfos[i].m_frame.m_ptexture, 100.0f, ImVec2(50, 50));
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

	EditorUI::InputText("Name", m_animName);

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

		updateAnim = true;
	}

	ImGui::SameLine();

	if (ImGui::Button("Save") == true && m_animName != "")
	{
		SaveAnim(m_animName);
	}

	ImGui::End();

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

		m_pgameObject->PlayAnimation("Anim");
	}
}

void AnimationTool::Destroy()
{
}

bool AnimationTool::SaveAnim(string animName)
{
	string filepath = GameManager::GetInstance()->GetWorkingDirectory() + "\\Resources\\Animations\\" + m_animName;

	int result = _mkdir(filepath.c_str());

	if (result != 0)
	{
		LOG("Failed to create the file to save the animation!");

		return false;
	}

	//Copy all the frames to the correct locations
	for (int i = 0; i < m_frameInfos.size(); ++i)
	{
		string frameName = animName + to_string(i) + ".dds";

		string tempPath = filepath + "\\" + frameName;
		wstring destPath = wstring(tempPath.begin(), tempPath.end());
		wstring sourcePath = GameManager::GetInstance()->GetWideWorkingDirectory() + L"\\" + m_frameInfos[i].m_filepath;

		if (CopyFile(sourcePath.c_str(), destPath.c_str(), true) == false)
		{
			LOG("Failed to copy one of the files of the animation!");
		}

	}

	ofstream textFile = ofstream(filepath + "\\" + animName + ".txt");
	
	if (textFile.is_open() == false)
	{
		LOG("Failed to create the text file for the animation!");

		return false;
	}

	textFile << m_frameInfos.size() << endl;
	
	for (int i = 0; i < m_frameInfos.size(); ++i)
	{
		textFile << m_frameInfos[i].m_frame.m_frameTime << endl;
	}

	textFile.close();

	return true;
}

#endif