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

PlayerGameObject* Scene::GetPlayerGameObjectUsingIdentifier(string& identifier)
{
	return  static_cast<PlayerGameObject*>(m_psceneGraph->GetGameObjectUsingIdentifier(identifier));
}

void Scene::SelectGameObjectUsingIdentifier(string& identifier)
{
	m_pcurrentlySelectedGameObject = m_psceneGraph->GetGameObjectUsingIdentifier(identifier);
}

void Scene::SelectGameObject(GameObject* pgameObject)
{
	m_pcurrentlySelectedGameObject = pgameObject;
}

GameObject* Scene::CreateGameObject(string identifier)
{
	return CreateGameObject(identifier, nullptr);
}

GameObject* Scene::CreateGameObject(string identifier, TreeNode* pparentNode)
{
	GameObject* gameObject = new GameObject(identifier);

	m_prootTreeNode = m_psceneGraph->GetRootNode();
	if (!m_prootTreeNode)
	{
		m_prootTreeNode = m_psceneGraph->NewNode(gameObject);
	}
	else
	{
		if (!pparentNode)
		{
			m_psceneGraph->AddSibling(m_prootTreeNode, gameObject);
		}
		else
		{
			m_psceneGraph->AddChild(pparentNode, gameObject);
		}
	}

	return gameObject;
}

GameObject* Scene::CreatePlayerGameObject(string identifier)
{
	return CreatePlayerGameObject(identifier, nullptr);
}

GameObject* Scene::CreatePlayerGameObject(string identifier, TreeNode* pparentNode)
{
	PlayerGameObject* gameObject = new PlayerGameObject(identifier);

	m_prootTreeNode = m_psceneGraph->GetRootNode();
	if (!m_prootTreeNode)
	{
		m_prootTreeNode = m_psceneGraph->NewNode(gameObject);
	}
	else
	{
		if (!pparentNode)
		{
			m_psceneGraph->AddSibling(m_prootTreeNode, gameObject);
		}
		else
		{
			m_psceneGraph->AddChild(pparentNode, gameObject);
		}
	}

	return gameObject;
}

void Scene::DeleteGameObjectUsingNode(TreeNode* pnode)
{
	if (!pnode)
	{
		return;
	}
	m_psceneGraph->DeleteGameObjectUsingNode(pnode);
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

void Scene::DeleteSelectedGameObject()
{
	unordered_map<string, GameObject*> gameObjectList = m_psceneGraph->GetAllGameObjects();
	for (unordered_map<string, GameObject*>::iterator it = gameObjectList.begin(); it != gameObjectList.end(); ++it)
	{
		//it->second->Render(renderStruct);
		if (it->second == m_pcurrentlySelectedGameObject)
		{
			m_psceneGraph->DeleteGameObjectUsingIdentifier(it->first);
		}
	}
}

string& Scene::GetSceneName()
{
	return m_sceneIdentifier;
}
