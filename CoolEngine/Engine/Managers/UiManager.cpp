#include "UiManager.h"
#include "Engine/GameUI/GameUIComponent.h"
#include "Engine/GameUI/UICanvas.h"
#include "Engine/GameUI/TextComponent.h"
#include "Engine/GameUI/ImageComponent.h"
#include "Engine/Graphics/Mesh.h"

#include <stack>

void UIManager::Init(ID3D11Device* pDevice)
{
	m_pDevice = pDevice;
}

void UIManager::CreateCanvas(string identifier, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation)
{
	CoolUUID uuid;
	CreateCanvas(identifier, uuid, position, scale, rotation);
}

void UIManager::CreateCanvas(string identifier, CoolUUID uuid, XMFLOAT3& position, XMFLOAT3& scale, XMFLOAT3& rotation)
{
	UICanvas* canvas = new UICanvas(identifier, uuid, position, scale, rotation);
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
		m_pselectedUINode->NodeObject->ShowEngineUI();
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
	vector<GameUIComponent*>& components = m_pUISceneGraph->GetAllGameObjects();

	TreeNode<GameUIComponent>* pnode = nullptr;

	if (m_prootTreeNode == nullptr)
	{
		data["GameUI"]["RootNode"].push_back("Null");
	}
	else
	{
		data["GameUI"]["RootNode"].push_back(*m_prootTreeNode->NodeObject->GetUUID());
	}

	for (int i = 0; i < components.size(); ++i)
	{
		pnode = m_pUISceneGraph->GetNodeUsingIdentifier(components[i]->GetIdentifier());

		if (pnode != nullptr)
		{
			pnode->NodeObject->Serialize(data);

			std::string uuidString = to_string(*pnode->NodeObject->GetUUID());

			if (pnode->Sibling == nullptr)
			{
				data["GameUI"][(int)pnode->NodeObject->GetComponentType()][uuidString]["Sibling"].push_back("Null");
			}
			else
			{
				data["GameUI"][(int)pnode->NodeObject->GetComponentType()][uuidString]["Sibling"].push_back(*pnode->Sibling->NodeObject->GetUUID());
			}

			if (pnode->PreviousSibling == nullptr)
			{
				data["GameUI"][(int)pnode->NodeObject->GetComponentType()][uuidString]["PreviousSibling"].push_back("Null");
			}
			else
			{
				data["GameUI"][(int)pnode->NodeObject->GetComponentType()][uuidString]["PreviousSibling"].push_back(*pnode->PreviousSibling->NodeObject->GetUUID());
			}

			if (pnode->PreviousParent == nullptr)
			{
				data["GameUI"][(int)pnode->NodeObject->GetComponentType()][uuidString]["PreviousParent"].push_back("Null");
			}
			else
			{
				data["GameUI"][(int)pnode->NodeObject->GetComponentType()][uuidString]["PreviousParent"].push_back(*pnode->PreviousParent->NodeObject->GetUUID());
			}

			if (pnode->Child == nullptr)
			{
				data["GameUI"][(int)pnode->NodeObject->GetComponentType()][uuidString]["Child"].push_back("Null");
			}
			else
			{
				data["GameUI"][(int)pnode->NodeObject->GetComponentType()][uuidString]["Child"].push_back(*pnode->PreviousParent->NodeObject->GetUUID());
			}
		}
	}
}

void UIManager::Deserialize(nlohmann::json& data)
{
	m_pUISceneGraph->DeleteNode(m_prootTreeNode);

	//First loop through and create all the objects
	std::unordered_map<uint64_t, GameUIComponent*> components;

	for (nlohmann::json::iterator it = data["GameUI"].begin(); it != data["GameUI"].end(); ++it)
	{
		switch (std::stoi(it.key()))
		{
		case (int)(UIComponentType::CANVAS | UIComponentType::BASE):
		{
			for (nlohmann::json::iterator guidIt = data["GameUI"][(int)UIComponentType::CANVAS | (int)UIComponentType::BASE].begin(); guidIt != data["GameUI"][(int)UIComponentType::CANVAS | (int)UIComponentType::BASE].end(); ++guidIt)
			{
				components[stoi(it.key())] = new UICanvas(data, CoolUUID(stoi(it.key())));
			}
		}
			break;

		case (int)UIComponentType::IMAGE | (int)UIComponentType::BASE:
		{
			for (nlohmann::json::iterator guidIt = data["GameUI"][(int)UIComponentType::IMAGE | (int)UIComponentType::BASE].begin(); guidIt != data["GameUI"][(int)UIComponentType::IMAGE | (int)UIComponentType::BASE].end(); ++guidIt)
			{
				components[stoi(it.key())] = new ImageComponent(data, CoolUUID(stoi(it.key())));
			}
		}
			break;

		case (int)UIComponentType::TEXT | (int)UIComponentType::BASE:
		{
			for (nlohmann::json::iterator guidIt = data["GameUI"][(int)UIComponentType::TEXT | (int)UIComponentType::BASE].begin(); guidIt != data["GameUI"][(int)UIComponentType::TEXT | (int)UIComponentType::BASE].end(); ++guidIt)
			{
				components[stoi(it.key())] = new TextComponent(data, CoolUUID(stoi(it.key())), m_pDevice);
			}
		}
			break;

		case (int)UIComponentType::BASE:
		{
			for (nlohmann::json::iterator guidIt = data["GameUI"][(int)UIComponentType::BASE].begin(); guidIt != data["GameUI"][(int)UIComponentType::BASE].end(); ++guidIt)
			{
				components[stoi(it.key())] = new GameUIComponent(data, CoolUUID(stoi(it.key())));
			}
		}
			break;

		default:
			LOG("A UI component was saved that doesn't correspond to an existing type!");
			break;
		}
	}

	TreeNode<GameUIComponent>* pnode = nullptr;
	GameUIComponent* pcomponent = nullptr;

	//Next loop through and insert into scene graph
	for (int i = 0; i < components.size(); ++i)
	{
		if (*components[i]->GetUUID() == data["GameUI"]["RootNode"])
		{
			pnode = m_pUISceneGraph->NewNode(components[i]);

			break;
		}
	}

	pcomponent = components[data["GameUI"]["RootNode"]];
	pnode = m_pUISceneGraph->NewNode(pcomponent);

	std::stack<TreeNode<GameUIComponent>*> toPush;
	toPush.push(pnode);

	while (toPush.size() != 0)
	{
		pnode = toPush.top();
		toPush.pop();

		if (data["GameUI"][(int)pnode->NodeObject->GetComponentType()][*pnode->NodeObject->GetUUID()]["Sibling"] != "Null")
		{
			pcomponent = components[data["GameUI"][(int)pnode->NodeObject->GetComponentType()][*pnode->NodeObject->GetUUID()]["Sibling"]];
			
			toPush.push(m_pUISceneGraph->AddSibling(pnode, pcomponent));
		}

		if (data["GameUI"][(int)pnode->NodeObject->GetComponentType()][*pnode->NodeObject->GetUUID()]["Child"] != "Null")
		{
			pcomponent = components[data["GameUI"][(int)pnode->NodeObject->GetComponentType()][*pnode->NodeObject->GetUUID()]["Child"]];

			toPush.push(m_pUISceneGraph->AddSibling(pnode, pcomponent));
		}
	}
}

vector<GameUIComponent*>& UIManager::GetAllUIComponents()
{
    return m_pUISceneGraph->GetAllGameObjects();
}