#include "GameManager.h"

#include "Engine/GameObjects/GameObject.h"
#include "Engine/Scene/Scene.h"
#include "SceneGraph.h"

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

//void GameManager::Render(RenderStruct& renderStruct)
//{
//    if (!m_pcurrentScene)
//    {
//        return;
//    }
//
//    m_pcurrentScene->Render(renderStruct);
//}

//void GameManager::ChangeScene(string sceneIdentifier)
//{
//    unordered_map<string, Scene*>::iterator iterator;
//
//    iterator = m_sceneMap.find(sceneIdentifier);
//
//    if (iterator->second)
//    {
//        m_pcurrentScene = iterator->second;
//    }
//}

void GameManager::DeleteSceneUsingIdentifier(string sceneIdentifier)
{
    m_sceneMap.erase(sceneIdentifier);
}

void GameManager::DeleteSelectedScene()
{
    m_sceneMap.erase(m_pcurrentScene->GetSceneIdentifier());
}

Scene* GameManager::GetCurrentScene()
{
    return m_pcurrentScene;
}

void GameManager::SelectScene(Scene* pscene)
{
    m_pcurrentScene = pscene;
}

void GameManager::DeleteScene(Scene* pscene)
{
    m_sceneMap.erase(pscene->GetSceneIdentifier());
}

unordered_map<string, GameObject*>& GameManager::GetAllGameObjects()
{
    return m_pcurrentScene->GetAllGameObjects();
}

GameObject* GameManager::GetGameObjectUsingIdentifier(string& identifier)
{
    return m_pcurrentScene->GetGameObjectUsingIdentifier(identifier);
}

void GameManager::SelectGameObjectUsingIdentifier(string& identifier)
{
    m_pcurrentScene->SelectGameObjectUsingIdentifier(identifier);
}

void GameManager::SelectGameObject(GameObject* pgameObject)
{
    m_pcurrentScene->SelectGameObject(pgameObject);
}

void GameManager::SelectGameObjectUsingTreeNode(TreeNode* pnode)
{
    m_pcurrentScene->SelectGameObjectUsingTreeNode(pnode);
}

GameObject* GameManager::CreateGameObject(string identifier)
{
    return m_pcurrentScene->CreateGameObject(identifier);
}

void GameManager::DeleteSelectedGameObject()
{
    m_pcurrentScene->DeleteSelectedGameObject();
}

void GameManager::DeleteGameObjectUsingIdentifier(string identifier)
{
    m_pcurrentScene->DeleteGameObjectUsingIdentifier(identifier);
}

TreeNode* GameManager::GetRootTreeNode()
{
    return m_pcurrentScene->GetRootTreeNode();
}

TreeNode* GameManager::GetTreeNode(GameObject* pgameObject)
{
    return m_pcurrentScene->GetTreeNode(pgameObject);
}

string& GameManager::GetCurrentSceneName()
{
    return m_pcurrentScene->GetSceneIdentifier();
}

unordered_map<string, GameObject*>& GameManager::GetAllGameObjectsInCurrentScene()
{
    return m_pcurrentScene->GetAllGameObjects();
}

unordered_map<string, Scene*> GameManager::GetSceneList()
{
    return m_sceneMap;
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
