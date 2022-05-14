#include "GameManager.h"

#include "Engine/GameObjects/GameObject.h"
#include "Engine/GameObjects/WeaponGameObject.h"
#include "Engine/GameObjects/RangedWeaponGameObject.h"
#include "Engine/GameObjects/MeleeWeaponGameObject.h"
#include "Engine/GameObjects/InteractableGameObject.h"
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
	vector<GameObject*> gameObjectsInScene = m_pcurrentScene->GetAllGameObjects();

	for (size_t i = 0; i < gameObjectsInScene.size(); i++)
	{
		std::string typeIdentifier = std::to_string((int)gameObjectsInScene[i]->GetGameObjectType());
		gameObjectsInScene[i]->Serialize(data[typeIdentifier][std::to_string(*gameObjectsInScene[i]->m_UUID)]);
	}

	TreeNode<GameObject>* pnode = nullptr;


	if (m_pcurrentScene->GetRootTreeNode() == nullptr)
	{
		data["RootNode"] = -1;
	}
	else
	{
		data["RootNode"] = *m_pcurrentScene->GetRootTreeNode()->NodeObject->m_UUID;
	}

	for (int i = 0; i < gameObjectsInScene.size(); ++i)
	{
		pnode = m_pcurrentScene->GetTreeNode(gameObjectsInScene[i]);

		if (pnode != nullptr)
		{
			std::string uuidString = to_string(*pnode->NodeObject->m_UUID);

			if (pnode->Sibling == nullptr)
			{
				data[to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["Sibling"] = -1;
			}
			else
			{
				data[to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["Sibling"] = *pnode->Sibling->NodeObject->m_UUID;
			}

			if (pnode->PreviousSibling == nullptr)
			{
				data[to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["PreviousSibling"] = -1;
			}
			else
			{
				data[to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["PreviousSibling"] = *pnode->PreviousSibling->NodeObject->m_UUID;
			}

			if (pnode->PreviousParent == nullptr)
			{
				data[to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["PreviousParent"] = -1;
			}
			else
			{
				data[to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["PreviousParent"] = *pnode->PreviousParent->NodeObject->m_UUID;
			}

			if (pnode->Child == nullptr)
			{
				data[to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["Child"] = -1;
			}
			else
			{
				data[to_string((int)pnode->NodeObject->GetGameObjectType())][uuidString]["Child"] = *pnode->Child->NodeObject->m_UUID;
			}
		}
	}



}

void GameManager::Deserialize(nlohmann::json& data)
{
    std::unordered_map<uint64_t, GameObject*> gameObjects;

	for (nlohmann::json::const_iterator typeIt = data.begin(); typeIt != data.end(); ++typeIt)
	{
		if (typeIt.key() == "RootNode" || typeIt.key() == "AudioManager" || typeIt.key() == "GraphicsManager" || typeIt.key() == "GameUI" || typeIt.key() == "FontManager")
		{
			continue;
		}

		for (nlohmann::json::const_iterator objectIt = data[typeIt.key()].begin(); objectIt != data[typeIt.key()].end(); ++objectIt)
		{
			CoolUUID uuid = CoolUUID(stoull(objectIt.key()));
			std::string uuidString = to_string(*uuid);

			switch ((AccumlateType)stoi(typeIt.key()))
			{
			case AccumlateType::BASE:
				gameObjects[*uuid] = new GameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::RENDERABLE:
				gameObjects[*uuid] = new RenderableGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::COLLIDABLE:
				gameObjects[*uuid] = new CollidableGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::COLLIDABLE_RENDERERABLE:
				gameObjects[*uuid] = new RenderableCollidableGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::CHARACTER:
				gameObjects[*uuid] = new CharacterGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::ENEMY:
				gameObjects[*uuid] = new EnemyGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::PLAYER:
				gameObjects[*uuid] = new PlayerGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::PARTICLE_SYSTEM:
				gameObjects[*uuid] = new ParticleSystem(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::INTERACTABLE:
				gameObjects[*uuid] = new InteractableGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::WEAPON:
				gameObjects[*uuid] = new WeaponGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::MELEE_WEAPON:
				gameObjects[*uuid] = new MeleeWeaponGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			case AccumlateType::RANGE_WEAPON:
				gameObjects[*uuid] = new RangedWeaponGameObject(data[typeIt.key()][uuidString], uuid);
				gameObjects[*uuid]->m_UUID = uuid;
				break;

			default:
				LOG("Tried to deserialize an object that doesn't have a correct object type!");
				break;
			}
		}
	}

	TreeNode<GameObject>* pnode = nullptr;
	GameObject* pcomponent = nullptr;

	pcomponent = gameObjects[data["RootNode"]];
	pnode = m_pcurrentScene->m_psceneGraph->NewNode(pcomponent);

	std::stack<TreeNode<GameObject>*> toPush;
	toPush.push(pnode);

	while (toPush.size() != 0)
	{
		pnode = toPush.top();
		toPush.pop();

		uint64_t siblingCheck = data[std::to_string((int)pnode->NodeObject->GetGameObjectType())][std::to_string(*pnode->NodeObject->m_UUID)]["Sibling"];
		if (siblingCheck != -1)
		{
			pcomponent = gameObjects[siblingCheck];
			toPush.push(m_pcurrentScene->m_psceneGraph->AddSibling(pnode, pcomponent));
		}
		
		uint64_t childCheck = data[std::to_string((int)pnode->NodeObject->GetGameObjectType())][std::to_string(*pnode->NodeObject->m_UUID)]["Child"];
		if (childCheck != -1)
		{
			pcomponent = gameObjects[childCheck];
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
