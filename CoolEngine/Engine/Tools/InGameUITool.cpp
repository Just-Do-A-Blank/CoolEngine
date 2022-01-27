#if EDITOR

#include "InGameUITool.h"
#include "Engine/GameUI/GameUIComponent.h"
#include "Engine/Managers/UiManager.h"
#include "Engine/GameUI/ImageComponent.h"

#include <direct.h>
#include <fstream>

void InGameUITool::Init(ID3D11Device* pdevice)
{
	ToolBase::Init(pdevice);

	XMFLOAT3 pos = XMFLOAT3(0, 0, 0);
	XMFLOAT3 scale = XMFLOAT3(100, 100, 1);

	/*m_pgameObject = GameManager::GetInstance()->CreateGameObject<RenderableGameObject>("AnimModel");
	m_pgameObject->GetTransform()->SetPosition(pos);
	m_pgameObject->GetTransform()->SetScale(scale);*/
}

void InGameUITool::Update()
{
	//m_pgameObject->Update();
}

void InGameUITool::Render()
{
	bool updateAnim = false;

	//UIManager::GetInstance()->Render()

	ImGui::Begin("Game UI");

	DrawUIWindow();


	ImGui::End();

	

	//if (updateAnim)
	//{
	//	SpriteAnimation anim;

	//	m_frames.clear();

	//	/*for (int i = 0; i < m_frameInfos.size(); ++i)
	//	{
	//		m_frames.push_back(m_frameInfos[i].m_frame);
	//	}*/

	//	anim.SetFrames(&m_frames);

	//	/*m_pgameObject->RemoveAnimation("Anim");

	//	m_pgameObject->AddAnimation("Anim", anim);

	//	m_pgameObject->PlayAnimation("Anim");*/
	//}
}

void InGameUITool::Destroy()
{
}

void InGameUITool::DrawUIWindow()
{
	/*if (puiManager->GetCanvasList().size() == 0)
	{
		return;
	}*/
	UIManager* puiManager = UIManager::GetInstance();

	//ImGui::Begin("UI Graph", nullptr, ImGuiWindowFlags_MenuBar);

	static int selected = -1;

	m_base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	int nodeCount = -1;


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
		}
	}

	if (m_gameObjectNodeClicked != -1)
	{
		if (ImGui::Button("Delete") == true)
		{
			puiManager->DeleteSelectedUIComponent();
			puiManager->SelectUIObject(nullptr);
			m_gameObjectNodeClicked = -1;
		}
	}

	TreeNode<GameUIComponent>* prootNode = puiManager->GetRootTreeNode();

	ImGui::Begin("UI SceneGraph");
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

	bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)nodeCount, node_flags, pcurrentNode->GameObject->GetIdentifier().c_str(), nodeCount);
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

bool InGameUITool::SaveAnim(string animName)
{
	//string filepath = GameManager::GetInstance()->GetWorkingDirectory() + "\\Resources\\Animations\\" + m_animName;

	//int result = _mkdir(filepath.c_str());

	//if (result != 0)
	//{
	//	LOG("Failed to create the file to save the animation!");

	//	return false;
	//}

	////Copy all the frames to the correct locations
	//for (int i = 0; i < m_frameInfos.size(); ++i)
	//{
	//	string frameName = animName + to_string(i) + ".dds";

	//	string tempPath = filepath + "\\" + frameName;
	//	wstring destPath = wstring(tempPath.begin(), tempPath.end());
	//	wstring sourcePath = GameManager::GetInstance()->GetWideWorkingDirectory() + L"\\" + m_frameInfos[i].m_filepath;

	//	if (CopyFile(sourcePath.c_str(), destPath.c_str(), true) == false)
	//	{
	//		LOG("Failed to copy one of the files of the animation!");
	//	}

	//}

	//ofstream textFile = ofstream(filepath + "\\" + animName + ".txt");
	//
	//if (textFile.is_open() == false)
	//{
	//	LOG("Failed to create the text file for the animation!");

	//	return false;
	//}

	//textFile << m_frameInfos.size() << endl;
	//
	//for (int i = 0; i < m_frameInfos.size(); ++i)
	//{
	//	textFile << m_frameInfos[i].m_frame.m_frameTime << endl;
	//}

	//textFile.close();

	return true;
}

#endif