#include "UiManager.h"
#include "Engine/GameUI/GameUIComponent.h"
#include "Engine/GameUI/UICanvas.h"
#include "Engine/GameUI/TextComponent.h"
#include "Engine/GameUI/ImageComponent.h"

void UIManager::Init(ID3D11Device* pDevice)
{
	m_pDevice = pDevice;
}

void UIManager::CreateCanvas(string identifier, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation)
{
	UICanvas* canvas = new UICanvas(identifier, position, scale, rotation);
	if (!m_pUISceneGraph)
	{
		m_pUISceneGraph = new SceneGraph<GameUIComponent>();
	}

	m_prootTreeNode = m_pUISceneGraph->GetRootNode();
	if (!m_prootTreeNode)
	{
		m_prootTreeNode = m_pUISceneGraph->NewNode(canvas);
	}
	else
	{
        m_pUISceneGraph->AddSibling(m_prootTreeNode, canvas);
	}
}

void UIManager::SelectUIObjectUsingIdentifier(string identifier)
{
	m_pselectedUINode = m_pUISceneGraph->GetNodeUsingIdentifier(identifier);
}

void UIManager::SelectUIObject(GameUIComponent* pgameObject)
{
	if (pgameObject)
	{
		SelectUIObjectUsingIdentifier(pgameObject->GetIdentifier());
	}
	else
	{
		m_pselectedUINode = nullptr;
	}
}

void UIManager::SelectUIObjectUsingTreeNode(TreeNode<GameUIComponent>* pnode)
{
	if (!pnode)
	{
		m_pselectedUINode = nullptr;
		return;
	}

	m_pselectedUINode = pnode;
}

TreeNode<GameUIComponent>* UIManager::GetRootTreeNode()
{
	return m_pUISceneGraph->GetRootNode();
}

void UIManager::Render(RenderStruct& renderStruct)
{
	if (!m_pUISceneGraph)
	{
		return;
	}

	if (m_pselectedUINode)
	{
		m_pselectedUINode->GameObject->ShowEngineUI();
	}

	vector<GameUIComponent*> uiComponentList = m_pUISceneGraph->GetAllGameObjects();
	for (int i = 0; i < uiComponentList.size(); ++i)
	{
		uiComponentList[i]->Render(renderStruct);
	}
}

void UIManager::DeleteSelectedUIComponent()
{
	m_pUISceneGraph->DeleteGameObjectUsingNode(m_pselectedUINode);
	m_pselectedUINode = nullptr;
}

vector<GameUIComponent*>& UIManager::GetAllUIComponents()
{
    return m_pUISceneGraph->GetAllGameObjects();
}