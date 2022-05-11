#if EDITOR

#include "ToolBase.h"
#include "Engine/Managers/GameManager.h"

#include "Engine/Includes/IMGUI/imgui.h"

void ToolBase::Init(ID3D11Device* pdevice)
{
	m_pdevice = pdevice;

	m_previousSceneIdentifier = GameManager::GetInstance()->GetCurrentSceneName();

	GameManager::GetInstance()->CreateScene("ToolScene");
	GameManager::GetInstance()->SelectSceneUsingIdentifier("ToolScene");
}

void ToolBase::Update()
{
}

void ToolBase::Render()
{
}

void ToolBase::Destroy()
{
	m_exitTool = true;

	GameManager::GetInstance()->SelectSceneUsingIdentifier(m_previousSceneIdentifier);

	GameManager::GetInstance()->DeleteSceneUsingIdentifier("ToolScene");
}

const bool& ToolBase::HasToolExited() const
{
	return m_exitTool;
}

void ToolBase::CreateMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Back"))
		{
			Destroy();

			ImGui::EndMenu();
		}


		ImGui::EndMenuBar();
	}
}

#endif