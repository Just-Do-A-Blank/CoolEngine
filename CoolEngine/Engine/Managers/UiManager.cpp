#include "UiManager.h"
#include "Engine/GameUI/GameUIComponent.h"
#include "Engine/GameUI/UICanvas.h"
#include "Engine/GameUI/TextComponent.h"
#include "Engine/GameUI/ImageComponent.h"
#include "Engine/Graphics/Mesh.h"

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

#if EDITOR
	if (m_pselectedUINode)
	{
		m_pselectedUINode->GameObject->ShowEngineUI();
	}
#endif

	Mesh* pMesh = GraphicsManager::GetInstance()->GetMesh(QUAD_MESH_NAME);

	ID3D11Buffer* pVertexBuffer = pMesh->GetVertexBuffer();
	UINT stride = pMesh->GetVBStride();
	UINT offset = pMesh->GetVBOffset();

	renderStruct.m_pcontext->IASetInputLayout(GraphicsManager::GetInstance()->GetInputLayout(GraphicsManager::InputLayouts::POS_TEX));

	renderStruct.m_pcontext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	renderStruct.m_pcontext->IASetIndexBuffer(pMesh->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);

	ID3D11Buffer* pperFrame = GraphicsManager::GetInstance()->m_pperFrameCB->GetBuffer();

	renderStruct.m_pcontext->VSSetConstantBuffers((int)GraphicsManager::CBOrders::PER_FRAME, 1, &pperFrame);

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

void UIManager::Serialize(nlohmann::json& data)
{
}

void UIManager::Deserialize(nlohmann::json& data)
{
}

vector<GameUIComponent*>& UIManager::GetAllUIComponents()
{
    return m_pUISceneGraph->GetAllGameObjects();
}