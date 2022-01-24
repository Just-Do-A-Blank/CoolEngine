#include "UiCanvas.h"
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/GameUI/GameUIComponent.h"

void UICanvas::CreateEngineUI(ID3D11Device* pdevice)
{
}

UICanvas::UICanvas(string identifier, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation) : GameUIComponent(identifier, position, scale, rotation)
{
	m_canvasIdentifier = identifier;
	m_pUISceneGraph = new SceneGraph<GameUIComponent>();
	m_prootTreeNode = m_pUISceneGraph->NewNode(this);
}


unordered_map<string, GameUIComponent*>& UICanvas::GetAllGameUIComponents()
{
	return m_pUISceneGraph->GetAllGameObjects();
}

void UICanvas::Render(RenderStruct& renderStruct)
{
	for (int i = 0; i < GraphicsManager::GetInstance()->GetNumLayers(); ++i)
	{
		unordered_map<string, GameUIComponent*> uiElementList = m_pUISceneGraph->GetAllGameObjects();
		for (unordered_map<string, GameUIComponent*>::iterator it = uiElementList.begin(); it != uiElementList.end(); ++it)
		{
			if (it->second == this || it->second->IsRenderable() == false || it->second->GetLayer() != i)
			{
				continue;
			}

			it->second->Render(renderStruct);
		}
	}
}


void UICanvas::Update()
{
	for (int i = 0; i < GraphicsManager::GetInstance()->GetNumLayers(); ++i)
	{
		unordered_map<string, GameUIComponent*> uiElementList = m_pUISceneGraph->GetAllGameObjects();
		for (unordered_map<string, GameUIComponent*>::iterator it = uiElementList.begin(); it != uiElementList.end(); ++it)
		{
			it->second->Update();
		}
	}
}

void UICanvas::ShowEngineUI(ID3D11Device* pdevice)
{
}

void UICanvas::SetIsRenderable(bool& condition)
{
	m_isRenderable = condition;
}

const string& UICanvas::GetCanvasIdentifier()
{
	return m_canvasIdentifier;
}

Transform* UICanvas::GetTransform()
{
	return m_transform;
}

TreeNode<GameUIComponent>* UICanvas::GetRootTreeNode()
{
	return m_pUISceneGraph->GetRootNode();
}

TreeNode<GameUIComponent>* UICanvas::GetTreeNode(GameUIComponent* pgameUIComponent)
{
	return m_pUISceneGraph->GetNodeUsingIdentifier(pgameUIComponent->GetIdentifier());
}
