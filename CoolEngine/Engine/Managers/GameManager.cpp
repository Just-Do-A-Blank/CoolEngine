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

vector<GameObject*>& GameManager::GetAllGameObjectsInCurrentScene()
{
    return m_pcurrentScene->GetAllGameObjects();
}

void GameManager::Serialize(nlohmann::json& data)
{
    vector<GameObject*> gameObjectsInScene = m_pcurrentScene->GetAllGameObjects();
    TreeNode<GameObject>* treeNode = m_pcurrentScene->GetSceneGraph()->GetRootNode();

	TreeNode<GameObject>* pnode = nullptr;

    data[m_pcurrentScene->m_sceneIdentifier] = {};

  /*  if (treeNode == nullptr)
    {
        data[m_pcurrentScene->m_sceneIdentifier]["RootNode"].push_back("Null");
    }
    else
    {
        data[m_pcurrentScene->m_sceneIdentifier]["RootNode"].push_back(*treeNode->NodeObject->m_UUID);

    }*/
    for (size_t i = 0; i < gameObjectsInScene.size(); i++)
    {
        gameObjectsInScene[i]->Serialize(data[m_pcurrentScene->m_sceneIdentifier]);
    }

	/*for (int i = 0; i < gameObjectsInScene.size(); ++i)
	{
		pnode = m_pcurrentScene->GetGameObjectUsingIdentifier<GameObject>(gameObjectsInScene[i]->GetIdentifier());

		if (pnode != nullptr)
		{
			pnode->NodeObject->Serialize(data);

			std::string uuidString = to_string(pnode->NodeObject->GetUUID());

			if (pnode->Sibling == nullptr)
			{
				data[m_pcurrentScene->m_sceneIdentifier][(int)pnode->NodeObject->GetComponentType()][uuidString]["Sibling"].push_back("Null");
			}
			else
			{
				data[m_pcurrentScene->m_sceneIdentifier][(int)pnode->NodeObject->GetComponentType()][uuidString]["Sibling"].push_back(*pnode->Sibling->NodeObject->GetUUID());
			}

			if (pnode->PreviousSibling == nullptr)
			{
				data[m_pcurrentScene->m_sceneIdentifier][(int)pnode->NodeObject->GetComponentType()][uuidString]["PreviousSibling"].push_back("Null");
			}
			else
			{
				data[m_pcurrentScene->m_sceneIdentifier][(int)pnode->NodeObject->GetComponentType()][uuidString]["PreviousSibling"].push_back(*pnode->PreviousSibling->NodeObject->GetUUID());
			}

			if (pnode->PreviousParent == nullptr)
			{
				data[m_pcurrentScene->m_sceneIdentifier][(int)pnode->NodeObject->GetComponentType()][uuidString]["PreviousParent"].push_back("Null");
			}
			else
			{
				data[m_pcurrentScene->m_sceneIdentifier][(int)pnode->NodeObject->GetComponentType()][uuidString]["PreviousParent"].push_back(*pnode->PreviousParent->NodeObject->GetUUID());
			}

			if (pnode->Child == nullptr)
			{
				data[m_pcurrentScene->m_sceneIdentifier][(int)pnode->NodeObject->GetComponentType()][uuidString]["Child"].push_back("Null");
			}
			else
			{
				data[m_pcurrentScene->m_sceneIdentifier][(int)pnode->NodeObject->GetComponentType()][uuidString]["Child"].push_back(*pnode->PreviousParent->NodeObject->GetUUID());
			}
		}*/
	/*}*/




}

void GameManager::Deserialize(nlohmann::json& data)
{
    std::unordered_map<uint64_t, GameObject*> gameObjects;


	for (auto it = data.begin(); it != data.end(); ++it)
	{
		for (size_t i = 0; i < it.value()["GUID"].size(); i++)
		{
			int test = it.value().size();
			int key = std::stoi(it.key());
			if ((int)GameObjectType::BASE <= key && key < (int)GameObjectType::RENDERABLE)
			{
				json j = data[it.key()]["GUID"];
				CoolUUID id = CoolUUID(j[i]);
				gameObjects[*id] = new GameObject(data[it.key()], i);
			}
			else if ((int)GameObjectType::RENDERABLE < key && key < (int)GameObjectType::COLLIDABLE)
			{
				json j = data[it.key()]["GUID"];
				CoolUUID id = CoolUUID(j[i]);
				gameObjects[*id] = new RenderableGameObject(data[it.key()], i);
			}
			else if ((int)GameObjectType::COLLIDABLE < key && key < (int)GameObjectType::CHARACTER)
			{
				json j = data[it.key()]["GUID"];
				CoolUUID id = CoolUUID(j[i]);
				gameObjects[*id] = new CollidableGameObject(data[it.key()], i);
			}
			else if ((int)GameObjectType::CHARACTER < key && key < (int)GameObjectType::ENEMY)
			{
				json j = data[it.key()]["GUID"];
				CoolUUID id = CoolUUID(j[i]);
				gameObjects[*id] = new CharacterGameObject(data[it.key()], i);
			}
			else if ((int)GameObjectType::ENEMY < key && key < (int)GameObjectType::PARTICLESYSTEM)
			{
				//json j = data[it.key()]["GUID"];
				//CoolUUID id = CoolUUID(j[i]);
				//gameObjects[*id] = new (data[it.key()], i);
			}
			else if ((int)GameObjectType::PARTICLESYSTEM < key && key < (int)GameObjectType::INTERACTABLE)
			{
				json j = data[it.key()]["GUID"];
				CoolUUID id = CoolUUID(j[i]);
				gameObjects[*id] = new PlayerGameObject(data[it.key()], i);
			}


			//switch (std::stoi(it.key()))
			//{
			//case  (int)GameObjectType::RENDERABLE | (int)GameObjectType::BASE:
   //             gameObjects[std::stoi(it.key())] = new GameObject(data[it.key()], i);
			//	break;
			//case  (int)GameObjectType::RENDERABLE | (int)GameObjectType::COLLIDABLE:
   //             gameObjects[std::stoi(it.key())] = new RenderableGameObject(data[it.key()], i);
			//	break;
			//case  (int)GameObjectType::COLLIDABLE | (int)GameObjectType::CHARACTER:
   //             gameObjects[std::stoi(it.key())] = new CollidableGameObject(data[it.key()], i);
			//	break;
			//case  (int)GameObjectType::CHARACTER | (int)GameObjectType::ENEMY:
   //             gameObjects[std::stoi(it.key())] = new CharacterGameObject(data[it.key()], i);
			//	break;
			//case (int)GameObjectType::ENEMY | (int)GameObjectType::PLAYER:

			//	break;
			//case  (int)GameObjectType::PLAYER | (int)GameObjectType::PARTICLESYSTEM:
   //             gameObjects[std::stoi(it.key())] = new PlayerGameObject(data[it.key()], i);
			//	break;
			//case  (int)GameObjectType::PARTICLESYSTEM | (int)GameObjectType::INTERACTABLE:
			//	/*particleSystem = new ParticleSystem(dataIn[it.key()], i);*/
			//	LOG("Type not compatable");
			//	break;
			//case  (int)GameObjectType::INTERACTABLE | (int)GameObjectType::WEAPON:
			//	LOG("Type not compatable");
			//	break;
			//case  (int)GameObjectType::WEAPON | (int)GameObjectType::COUNT:
			//	LOG("Type not compatable");
			//	break;
			//default:
			//	LOG("Can't determin Object Type");
			//	break;
			//}
		}
	}	


	for (auto it = data.begin(); it != data.end(); ++it)
	{
		for (size_t i = 0; i < it.value()["GUID"].size(); i++)
		{
			json j = data[it.key()]["GUID"];
			CoolUUID id = CoolUUID(j[i]);
			if (gameObjects[*id] != NULL)
			{
				m_pcurrentScene->m_psceneGraph->NewNode(gameObjects[*id]);
			}
		}
	}
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
