#include "Scene.h"

#include "Engine/GameObjects/GameObject.h"
#include "Engine/Managers/SceneGraph.h"

#include "Engine/Physics/Collision.h"

Scene::Scene(string identifier)
{
	m_sceneIdentifier = identifier;
	m_psceneGraph = new SceneGraph();
}

Scene::~Scene()
{
}

void Scene::Update()
{
	unordered_map<string, GameObject*> gameObjectList = m_psceneGraph->GetAllGameObjects();
	for (unordered_map<string, GameObject*>::iterator it = gameObjectList.begin(); it != gameObjectList.end(); ++it)
	{
		it->second->Update();
	}

	Collision::Update(gameObjectList);
}

void Scene::Render(RenderStruct& renderStruct)
{
	for (int i = 0; i < GraphicsManager::GetInstance()->GetNumLayers(); ++i)
	{
		unordered_map<string, GameObject*> gameObjectList = m_psceneGraph->GetAllGameObjects();
		for (unordered_map<string, GameObject*>::iterator it = gameObjectList.begin(); it != gameObjectList.end(); ++it)
		{
			if (it->second->IsRenderable() == false || it->second->GetLayer() != i)
			{
				continue;
			}

			it->second->Render(renderStruct);
		}
	}
}

unordered_map<string, GameObject*>& Scene::GetAllGameObjects()
{
	return m_psceneGraph->GetAllGameObjects();
}

GameObject* Scene::GetGameObjectUsingIdentifier(string& identifier)
{	
	return  m_psceneGraph->GetGameObjectUsingIdentifier(identifier);
}

void Scene::SelectGameObjectUsingIdentifier(string identifier)
{
	m_pselectedNode = m_psceneGraph->GetNodeUsingIdentifier(identifier);
	m_pselectedGameObject = m_pselectedNode->GameObject;
}

void Scene::SelectGameObject(GameObject* pgameObject)
{
	if (pgameObject)
	{
		SelectGameObjectUsingIdentifier(pgameObject->GetIdentifier());
	}
	else
	{
		m_pselectedNode = nullptr;
		m_pselectedGameObject = nullptr;
	}
}

void Scene::SelectGameObjectUsingTreeNode(TreeNode* pnode)
{
	if (!pnode)
	{
		m_pselectedNode = nullptr;
		m_pselectedGameObject = nullptr;
		return;
	}

	m_pselectedNode = pnode;
	m_pselectedGameObject = pnode->GameObject;
}

GameObject* Scene::CreateGameObject(string identifier)
{
	GameObject* gameObject = new GameObject(identifier);

	m_prootTreeNode = m_psceneGraph->GetRootNode();
	if (!m_prootTreeNode)
	{
		m_prootTreeNode = m_psceneGraph->NewNode(gameObject);
	}
	else
	{
		if (!m_pselectedGameObject)
		{
			m_psceneGraph->AddSibling(m_prootTreeNode, gameObject);
		}
		else
		{
			m_psceneGraph->AddChild(m_psceneGraph->GetNodeUsingIdentifier(m_pselectedGameObject->GetIdentifier()), gameObject);
		}
	}

	return gameObject;
}

void Scene::DeleteSelectedGameObject()
{
	if (!m_pselectedNode)
	{
		return;
	}
	m_psceneGraph->DeleteGameObjectUsingNode(m_pselectedNode);
}

void Scene::DeleteGameObjectUsingIdentifier(string identifier)
{
	m_psceneGraph->DeleteGameObjectUsingIdentifier(identifier);
}

TreeNode* Scene::GetRootTreeNode()
{
	return m_psceneGraph->GetRootNode();
}

TreeNode* Scene::GetTreeNode(GameObject* pgameObject)
{
	return m_psceneGraph->GetNodeUsingIdentifier(pgameObject->GetIdentifier());
}

string& Scene::GetSceneIdentifier()
{
	return m_sceneIdentifier;
}

GameObject* Scene::GetSelectedGameObject()
{
	return m_pselectedGameObject;
}
