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
	//m_pselectedGameObject = m_pselectedNode->GameObject;
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
		//m_pselectedGameObject = nullptr;
	}
}

void UIManager::SelectUIObjectUsingTreeNode(TreeNode<GameUIComponent>* pnode)
{
	if (!pnode)
	{
		m_pselectedUINode = nullptr;
		//m_pselectedGameObject = nullptr;
		return;
	}

	m_pselectedUINode = pnode;
	//m_pselectedGameObject = pnode->GameObject;
}

void UIManager::DeleteCanvasUsingIdentifier(string canvasIdentifier)
{
    //m_uiCanvasMap.erase(canvasIdentifier);
}

void UIManager::DeleteCanvas(UICanvas* pcanvas)
{
    DeleteCanvasUsingIdentifier(pcanvas->GetCanvasIdentifier());
}

void UIManager::DeleteSelectedCanvas()
{
    //DeleteCanvasUsingIdentifier(m_pcurrentCanvas->GetCanvasIdentifier());
}

TreeNode<GameUIComponent>* UIManager::GetRootTreeNode()
{
	return m_pUISceneGraph->GetRootNode();
}

void UIManager::Render(RenderStruct& renderStruct)
{
	if (m_pselectedUINode)
	{
		m_pselectedUINode->GameObject->ShowEngineUI();
	}

	vector<GameUIComponent*> uiComponentList = m_pUISceneGraph->GetAllGameObjects();
	for (int i = 0; i < uiComponentList.size(); ++i)
	{
		uiComponentList[i]->Render(renderStruct);
	}
    //m_pcurrentCanvas->Render(renderStruct);
}

//unordered_map<string, UICanvas*> UIManager::GetCanvasList()
//{
//    return m_uiCanvasMap;
//}

vector<GameUIComponent*>& UIManager::GetAllUIComponents()
{
    return m_pUISceneGraph->GetAllGameObjects();
}