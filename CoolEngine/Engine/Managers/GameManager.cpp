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

    data[m_pcurrentScene->m_sceneIdentifier] = {};

	vector<GameObject*> gameObjectsInScene = m_pcurrentScene->GetAllGameObjects();

	for (size_t i = 0; i < gameObjectsInScene.size(); i++)
	{
		std::string typeIdentifier = std::to_string((int)gameObjectsInScene[i]->GetGameObjectType());
		gameObjectsInScene[i]->Serialize(data[m_pcurrentScene->m_sceneIdentifier][typeIdentifier][std::to_string(*gameObjectsInScene[i]->m_UUID)]);
	}

	TreeNode<GameObject>* pnode = nullptr;


	if (m_pcurrentScene->GetRootTreeNode() == nullptr)
	{
		data[m_pcurrentScene->m_sceneIdentifier]["RootNode"].push_back("Null");
	}
	else
	{
		data[m_pcurrentScene->m_sceneIdentifier]["RootNode"].push_back(*m_pcurrentScene->GetRootTreeNode()->NodeObject->m_UUID);
	}

	for (int i = 0; i < gameObjectsInScene.size(); ++i)
	{
		pnode = m_pcurrentScene->GetTreeNode(gameObjectsInScene[i]);

		if (pnode != nullptr)
		{
			pnode->NodeObject->Serialize(data);

			std::string uuidString = to_string(*pnode->NodeObject->m_UUID);

			if (pnode->Sibling == nullptr)
			{
				data[m_pcurrentScene->m_sceneIdentifier][to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["Sibling"].push_back(0);
			}
			else
			{
				data[m_pcurrentScene->m_sceneIdentifier][to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["Sibling"].push_back(*pnode->Sibling->NodeObject->m_UUID);
			}

			if (pnode->PreviousSibling == nullptr)
			{
				data[m_pcurrentScene->m_sceneIdentifier][to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["PreviousSibling"].push_back(0);
			}
			else
			{
				data[m_pcurrentScene->m_sceneIdentifier][to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["PreviousSibling"].push_back(*pnode->PreviousSibling->NodeObject->m_UUID);
			}

			if (pnode->PreviousParent == nullptr)
			{
				data[m_pcurrentScene->m_sceneIdentifier][to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["PreviousParent"].push_back(0);
			}
			else
			{
				data[m_pcurrentScene->m_sceneIdentifier][to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["PreviousParent"].push_back(*pnode->PreviousParent->NodeObject->m_UUID);
			}

			if (pnode->Child == nullptr)
			{
				data[m_pcurrentScene->m_sceneIdentifier][to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["Child"].push_back(0);
			}
			else
			{
				data[m_pcurrentScene->m_sceneIdentifier][to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["Child"].push_back(*pnode->PreviousParent->NodeObject->m_UUID);
			}
		}
	}



}

void GameManager::Deserialize(nlohmann::json& data)
{
    std::unordered_map<uint64_t, GameObject*> gameObjects;


	for (auto it = data.begin(); it != data.end(); ++it)
	{
		for (size_t i = 0; i < it.value().size(); i++)
		{
			nlohmann::json::iterator idIterator = data[it.key()].begin();
			uint64_t iD;
			std::string uID;

			if (it.key() == "RootNode")
			{
				break;
			}
			for (size_t j = 0; j < i + 1; j++)
			{
				uID = idIterator.key();
				//https://stackoverflow.com/questions/42356939/c-convert-string-to-uint64-t
				std::istringstream iss(idIterator.key());
				iss >> iD;
				++idIterator;
			}

			int its = (int)AccumlateType::PLAYER;
			json inputData = data[it.key()];
			int outputs = std::stoi(it.key());
			switch ((AccumlateType)std::stoi(it.key()))
			{
			case AccumlateType::BASE:
				gameObjects[iD] = new GameObject(inputData[uID], iD);
				gameObjects[iD]->m_UUID = iD;
				break;
			case AccumlateType::RENDERABLE:
				gameObjects[iD] = new RenderableGameObject(inputData[uID], iD);
				gameObjects[iD]->m_UUID = iD;
				break;
			case AccumlateType::COLLIDABLE:
				gameObjects[iD] = new CollidableGameObject(inputData[uID], iD);
				gameObjects[iD]->m_UUID = iD;
				break;
			case AccumlateType::COLLIDABLE_RENDERERABLE:
				gameObjects[iD] = new RenderableCollidableGameObject(inputData[uID], iD);
				gameObjects[iD]->m_UUID = iD;
				break;
			case AccumlateType::CHARACTER:
				gameObjects[iD] = new CharacterGameObject(inputData[uID], iD);
				gameObjects[iD]->m_UUID = iD;
				break;
			case AccumlateType::ENEMY:
				gameObjects[iD] = new EnemyGameObject(inputData[uID], iD);
				gameObjects[iD]->m_UUID = iD;
				break;
			case AccumlateType::PLAYER:
				gameObjects[iD] = new PlayerGameObject(inputData[uID], iD);
				gameObjects[iD]->m_UUID = iD;
				break;
			case AccumlateType::PARTICLESYSTEM:
				gameObjects[iD] = new ParticleSystem(inputData[uID], iD);
				gameObjects[iD]->m_UUID = iD;
				break;
			case AccumlateType::INTERACTABLE:
				break;
			case AccumlateType::WEAPON:
				break;
			//case AccumlateType::TRIGGERABLE:
			//	break;
			//case AccumlateType::COUNT:
			//	break;
			default:
				break;
			}
		}
	}	

	TreeNode<GameObject>* pnode = nullptr;
	GameObject* pcomponent = nullptr;

	uint64_t target = data["RootNode"][0].get<uint64_t>();
	for (std::unordered_map<uint64_t, GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if (it->first == target)
		{
			pnode = m_pcurrentScene->m_psceneGraph->NewNode(it->second);
			break;
		}
	}


	pcomponent = gameObjects[target];
	pnode = m_pcurrentScene->m_psceneGraph->NewNode(pcomponent);

	std::stack<TreeNode<GameObject>*> toPush;
	toPush.push(pnode);

	while (toPush.size() != 0)
	{
		pnode = toPush.top();
		toPush.pop();

		int test = (int)pnode->NodeObject->GetGameObjectType();
		int siblingCheck = data[std::to_string((int)pnode->NodeObject->GetGameObjectType())][std::to_string(*pnode->NodeObject->m_UUID)]["Sibling"][0].get<int>();
		if (siblingCheck != 0)
		{
			pcomponent = gameObjects[data[std::to_string((int)pnode->NodeObject->GetGameObjectType())][std::to_string(*pnode->NodeObject->m_UUID)]["Sibling"][0].get<uint64_t>()];
			toPush.push(m_pcurrentScene->m_psceneGraph->AddSibling(pnode, pcomponent));
		}
		
		int childCheck = data[std::to_string((int)pnode->NodeObject->GetGameObjectType())][std::to_string(*pnode->NodeObject->m_UUID)]["Child"][0].get<uint64_t>();
		if (childCheck != 0)
		{
			pcomponent = gameObjects[data[std::to_string((int)pnode->NodeObject->GetGameObjectType())][std::to_string(*pnode->NodeObject->m_UUID)]["Child"][0].get<uint64_t>()];
			toPush.push(m_pcurrentScene->m_psceneGraph->AddSibling(pnode, pcomponent));
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
