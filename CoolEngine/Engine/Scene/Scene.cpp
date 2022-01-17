#include "Scene.h"

#include "Engine/GameObjects/GameObject.h"

Scene::Scene(string identifier)
{
	m_sceneIdentifier = identifier;
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

void Scene::CreateGameObject(string& identifier)
{
	GameObject* gameObject = new GameObject(identifier);

	m_gameObjectMap.insert(pair<string, GameObject*>(identifier, gameObject));
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
