#include "GameManager.h"

GameObject* GameManager::GetGameObject(string name)
{
    //return m_gameObjectMap[name];
}

Timer* GameManager::GetTimer()
{
    return &m_timer;
}

void GameManager::Update()
{
    if (!m_pcurrentScene)
    {
        return;
    }

    m_pcurrentScene->Update();

    //Get unordered_map of GameObjects
    unordered_map<string, GameObject*> gameObjects = m_pcurrentScene->GetAllGameObjects();
}

void GameManager::Render(RenderStruct& renderStruct)
{
    if (!m_pcurrentScene)
    {
        return;
    }

    m_pcurrentScene->Render(renderStruct);
}

void GameManager::ChangeScene(string sceneIdentifier)
{
    unordered_map<string, Scene*>::iterator iterator;

    iterator = m_sceneMap.find(sceneIdentifier);

    if (iterator->second)
    {
        m_pcurrentScene = iterator->second;
    }
}

void GameManager::DeleteScene(string sceneIdentifier)
{
    m_sceneMap.erase(sceneIdentifier);
}

void GameManager::CreateGameObject(string identifier)
{
    m_pcurrentScene->CreateGameObject(identifier);
}

void GameManager::DeleteGameObject(string identifier)
{
    m_pcurrentScene->DeleteGameObjectUsingIdentifier(identifier);
}

void GameManager::CreateScene(string sceneIdentifier)
{
    Scene* newScene = new Scene(sceneIdentifier);
    m_sceneMap.insert(pair<string, Scene*>(sceneIdentifier, newScene));

    if (!m_pcurrentScene)
    {
        m_pcurrentScene = newScene;
    }
}
