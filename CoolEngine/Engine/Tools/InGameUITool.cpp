#if EDITOR

#include "InGameUITool.h"
#include "Engine/GameUI/GameUIComponent.h"
#include "Engine/Managers/UiManager.h"
#include "Engine/GameUI/ImageComponent.h"
#include "Engine/FileIO/FileIO.h"
#include "Engine/ResourceDefines.h"

#include <direct.h>
#include <fstream>

void InGameUITool::Init(ID3D11Device* pdevice)
{
	ToolBase::Init(pdevice);

	XMFLOAT3 pos = XMFLOAT3(0, 0, 0);
	XMFLOAT3 scale = XMFLOAT3(100, 100, 1);
}

void InGameUITool::Update()
{
}

void InGameUITool::Render()
{
	bool updateAnim = false;

	DrawUIWindow();
}

void InGameUITool::Destroy()
{
}

void InGameUITool::DrawUIWindow()
{
	UIManager* puiManager = UIManager::GetInstance();

	static int selected = -1;

	m_base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	if (m_showUICreation == true)
	{
		ImGui::Begin("Game UI");

		EditorUI::InputText("Canvas Name", m_canvasName, 120);

		ImGui::SameLine();

		if (ImGui::Button("New Canvas") == true)
		{
			puiManager->CreateCanvas(m_canvasName, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
			m_canvasName = "";
		}

		EditorUI::InputText("Image UI Name", m_imageName, 120);

		ImGui::SameLine();

		if (ImGui::Button("New ImageUI") == true)
		{
			ImageComponent* imageUI = puiManager->CreateUIComponent<ImageComponent>(m_imageName, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(10.0f, 10.0f, 10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
			if (imageUI)
			{
				imageUI->Init(L"");
				m_imageName = "";

				m_showUICreation = false;
			}
		}

		EditorUI::InputText("Text UI Name", m_textName, 120);

		ImGui::SameLine();

		if (ImGui::Button("New TextUI") == true)
		{
			TextComponent* textUI = puiManager->CreateUIComponent<TextComponent>(m_textName, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
			if (textUI)
			{
				textUI->Init("New Text", "comicSans", 20, Colors::White, m_pdevice);
				m_textName = "";

				m_showUICreation = false;
			}
		}

		

		if (m_gameObjectNodeClicked != -1)
		{
			if (ImGui::Button("Delete") == true)
			{
				puiManager->DeleteSelectedUIComponent();
				puiManager->SelectUIObject(nullptr);
				m_gameObjectNodeClicked = -1;

				m_showUICreation = false;
			}
			ImGui::SameLine();
		}

		if (ImGui::Button("Save UI") == true)
		{
			SaveUI();
		}

		ImGui::SameLine();

		if (ImGui::Button("Load UI") == true)
		{
			LoadUI();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel") == true)
		{
			m_showUICreation = false;
		}

		ImGui::End();
	}

	TreeNode<GameUIComponent>* prootNode = puiManager->GetRootTreeNode();

	ImGui::Begin("UI SceneGraph", nullptr, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Create UI"))
		{
			m_showUICreation = true;

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	int nodeCount = -1;
	TraverseTree(prootNode, nodeCount);
	ImGui::End();
}

void InGameUITool::TraverseTree(TreeNode<GameUIComponent>* pcurrentNode, int& nodeCount)
{
	if (!pcurrentNode)
	{
		return;
	}

	UIManager* puiManager = UIManager::GetInstance();

	++nodeCount;
	ImGuiTreeNodeFlags node_flags = m_base_flags;
	const bool is_selected = (m_selectionMask & (1 << nodeCount)) != 0;


	if (is_selected)
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}

	bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)nodeCount, node_flags, pcurrentNode->NodeObject->GetIdentifier().c_str(), nodeCount);
	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	{
		if (nodeCount == m_gameObjectNodeClicked)
		{
			m_gameObjectNodeClicked = -1;
			puiManager->SelectUIObject(nullptr);
		}
		else
		{
			m_gameObjectNodeClicked = nodeCount;
			puiManager->SelectUIObjectUsingTreeNode(pcurrentNode);
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

bool InGameUITool::SaveUI()
{
	FileIO::SaveUI(GAME_UI_PATH, UIManager::GetInstance());
	return true;
}

bool InGameUITool::LoadUI()
{
	FileIO::LoadUI(GAME_UI_PATH, UIManager::GetInstance(), m_pdevice);
	return true;
}



#endif