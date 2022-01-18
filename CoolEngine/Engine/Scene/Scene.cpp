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
	for (unordered_map<string, GameObject*>::iterator it = m_gameObjectMap.begin(); it != m_gameObjectMap.end(); ++it)
	{
		it->second->Update();
	}

	Collision::Update(m_gameObjectMap);
}

void Scene::Render(RenderStruct& renderStruct)
{
	for (int i = 0; i < GraphicsManager::GetInstance()->GetNumLayers(); ++i)
	{
		for (unordered_map<string, GameObject*>::iterator it = m_gameObjectMap.begin(); it != m_gameObjectMap.end(); ++it)
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
	return m_gameObjectMap;
}

GameObject* Scene::GetGameObjectUsingIdentifier(string& identifier)
{
	unordered_map<string, GameObject*>::iterator it;
	it = m_gameObjectMap.find(identifier);
	return it->second;
}

void Scene::SelectGameObjectUsingIdentifier(string& identifier)
{
	unordered_map<string, GameObject*>::iterator it;
	it = m_gameObjectMap.find(identifier);
	m_pcurrentlySelectedGameObject = it->second;
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

	m_gameObjectMap.insert(pair<string, GameObject*>(identifier, gameObject));

	return gameObject;
}

bool& Scene::DeleteGameObject(TreeNode* pnode)
{
	bool deleteSuccessful = false;
	if (!pnode)
	{
		return deleteSuccessful;
	}
	deleteSuccessful = m_gameObjectMap.erase(pnode->GameObject->GetIdentifier());
	m_psceneGraph->DeleteGameObject(pnode);
	return deleteSuccessful;
}

TreeNode* Scene::GetRootTreeNode()
{
	return m_psceneGraph->GetRootNode();
}

TreeNode* Scene::GetTreeNode(GameObject* pgameObject)
{
	return m_psceneGraph->GetNodeUsingIdentifier(pgameObject->GetIdentifier());
}

void Scene::DeleteGameObjectUsingIdentifier(string& identifier)
{
	m_gameObjectMap.erase(identifier);
}

void Scene::DeleteSelectedGameObject()
{
	for (unordered_map<string, GameObject*>::iterator it = m_gameObjectMap.begin(); it != m_gameObjectMap.end(); ++it)
	{
		//it->second->Render(renderStruct);
		if (it->second == m_pcurrentlySelectedGameObject)
		{
			m_gameObjectMap.erase(it->first);
		}
	}
}

string& Scene::GetSceneName()
{
	return m_sceneIdentifier;
}
