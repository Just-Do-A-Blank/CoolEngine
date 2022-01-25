#include "GameManager.h"

#include "Engine/GameObjects/GameObject.h"
#include "Engine/Scene/Scene.h"
#include "SceneGraph.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "GraphicsManager.h"

#include <direct.h>

void GameManager::Init()
{
    char buffer[FILEPATH_BUFFER_SIZE];
    _getcwd(buffer, FILEPATH_BUFFER_SIZE);

    m_workingDirectory = string(buffer);

    m_wideWorkingDirectory = wstring(m_workingDirectory.begin(), m_workingDirectory.end());
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
}

void GameManager::Render(RenderStruct& renderStruct)
{
    if (!m_pcurrentScene)
    {
        return;
    }

    m_pcurrentScene->Render(renderStruct);
}

void GameManager::DeleteSceneUsingIdentifier(string sceneIdentifier)
{
    m_sceneMap.erase(sceneIdentifier);
}

void GameManager::DeleteSelectedScene()
{
    m_sceneMap.erase(m_pcurrentScene->GetSceneIdentifier());
}

string GameManager::GetWorkingDirectory()
{
    return m_workingDirectory;
}

wstring GameManager::GetWideWorkingDirectory()
{
    return m_wideWorkingDirectory;
}

Scene* GameManager::GetCurrentScene()
{
    return m_pcurrentScene;
}

void GameManager::SelectScene(Scene* pscene)
{
    m_pcurrentScene = pscene;
}

void GameManager::SelectSceneUsingIdentifier(string sceneIdentifier)
{
    if (m_sceneMap.count(sceneIdentifier) == 0)
    {
        LOG("Scene : " << sceneIdentifier << "; was not found in Scene Map ");
        return;
    }
    m_pcurrentScene = m_sceneMap.find(sceneIdentifier)->second;
}

void GameManager::DeleteScene(Scene* pscene)
{
    m_sceneMap.erase(pscene->GetSceneIdentifier());
}

unordered_map<string, GameObject*>& GameManager::GetAllGameObjects()
{
    return m_pcurrentScene->GetAllGameObjects();
}

void GameManager::SelectGameObjectUsingIdentifier(string& identifier)
{
    m_pcurrentScene->SelectGameObjectUsingIdentifier(identifier);
}

void GameManager::SelectGameObject(GameObject* pgameObject)
{
    m_pcurrentScene->SelectGameObject(pgameObject);
}

void GameManager::SelectGameObjectUsingTreeNode(TreeNode<GameObject>* pnode)
{
    m_pcurrentScene->SelectGameObjectUsingTreeNode(pnode);
}

void GameManager::DeleteSelectedGameObject()
{
    m_pcurrentScene->DeleteSelectedGameObject();
}

void GameManager::DeleteGameObjectUsingIdentifier(string identifier)
{
    m_pcurrentScene->DeleteGameObjectUsingIdentifier(identifier);
}

TreeNode<GameObject>* GameManager::GetRootTreeNode()
{
    return m_pcurrentScene->GetRootTreeNode();
}

TreeNode<GameObject>* GameManager::GetTreeNode(GameObject* pgameObject)
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

GameObject* GameManager::GetSelectedGameObject()
{
    return m_pcurrentScene->GetSelectedGameObject();
}
