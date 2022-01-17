#include "Scene.h"

#include "Engine/GameObjects/GameObject.h"
#include "Engine/Managers/SceneGraph.h"

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
}

//void Scene::Render(RenderStruct& renderStruct)
//{
//	for (unordered_map<string, GameObject*>::iterator it = m_gameObjectMap.begin(); it != m_gameObjectMap.end(); ++it)
//	{
//		//it->second->Render(renderStruct);
//	}
//}

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

GameObject* Scene::CreateGameObject(string identifier, GameObject* pparentObject)
{
	GameObject* gameObject = new GameObject(identifier);

	if (pparentObject)
	{
		m_psceneGraph->AddSibling(m_psceneGraph->GetNodeUsingIdentifier(pparentObject->GetIdentifier()), gameObject);
	}
	else
	{
		m_psceneGraph->NewNode(gameObject);
	}

	m_gameObjectMap.insert(pair<string, GameObject*>(identifier, gameObject));

	return gameObject;
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
