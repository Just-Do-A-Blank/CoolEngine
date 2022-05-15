#include "GameManager.h"

#include "Engine/GameObjects/GameObject.h"
#include "Engine/Scene/Scene.h"
#include "SceneGraph.h"
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

void GameManager::BeginPlay()
{
	m_pplayScene = new Scene(m_pcurrentScene->m_sceneIdentifier);

	vector<TreeNode<GameObject>*> gameObjectNodeList = m_pcurrentScene->GetSceneGraph()->GetAllNodes();
	for (int it = 0; it < gameObjectNodeList.size(); ++it)
	{
		switch (gameObjectNodeList[it]->NodeObject->m_gameObjectType)
		{
		case GameObjectType::RENDERABLE | GameObjectType::BASE:
			if (gameObjectNodeList[it]->PreviousParent)
			{
				TreeNode<GameObject>* parentNode = m_pplayScene->GetTreeNode(gameObjectNodeList[it]->PreviousParent->NodeObject);
				m_pplayScene->CopyGameObject<RenderableGameObject>(*(dynamic_cast<RenderableGameObject*>(gameObjectNodeList[it]->NodeObject)), parentNode);
			}
			else if (gameObjectNodeList[it]->PreviousSibling)
			{
				TreeNode<GameObject>* previousSiblingNode = m_pplayScene->GetTreeNode(gameObjectNodeList[it]->PreviousSibling->NodeObject);
				m_pplayScene->CopyGameObject<RenderableGameObject>(*(dynamic_cast<RenderableGameObject*>(gameObjectNodeList[it]->NodeObject)), nullptr, previousSiblingNode);
			}
			else
			{
				m_pplayScene->CopyGameObject<RenderableGameObject>(*(dynamic_cast<RenderableGameObject*>(gameObjectNodeList[it]->NodeObject)));
			}
			break;
		}
	}
}

void GameManager::EndPlay()
{
	m_pplayScene->DeleteGameObjectUsingNode(m_pplayScene->m_prootTreeNode);
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

vector<GameObject*>& GameManager::GetAllGameObjects()
{
    return m_pcurrentScene->GetAllGameObjects();
}

CameraGameObject* GameManager::GetCamera()
{
	return m_pcamera;
}

void GameManager::SetCamera(CameraGameObject* pcamera)
{
	m_pcamera = pcamera;
}

void GameManager::DeleteGameObjectUsingNode(TreeNode<GameObject>* currentNode)
{
	if (!currentNode)
	{
		return;
	}
    m_pcurrentScene->DeleteGameObjectUsingNode(currentNode);
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

vector<GameObject*>& GameManager::GetAllGameObjectsInCurrentScene()
{
    return m_pcurrentScene->GetAllGameObjects();
}

void GameManager::Serialize(nlohmann::json& data)
{

}

void GameManager::Deserialize(nlohmann::json& data)
{
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
	if (m_pcurrentScene == nullptr)
	{
		return nullptr;
	}

    return m_pcurrentScene->GetSelectedGameObject();
}
