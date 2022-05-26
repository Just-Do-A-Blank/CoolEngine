#include "Scene.h"
#include "Engine/GameObjects/RenderableGameObject.h"
#include "Engine/Managers/SceneGraph.h"
#include "Engine/Physics/Collision.h"
#include "Engine/GameUI/GameUIComponent.h"
#include "Engine/GameObjects/CameraGameObject.h"

Scene::Scene(string identifier)
{
	m_sceneIdentifier = identifier;
	m_psceneGraph = new SceneGraph<GameObject>();
}

Scene::~Scene()
{

}

void Scene::Start()
{
    vector<GameObject*>& gameObjectList = m_psceneGraph->GetAllNodeObjects();
    for (int it = 0; it < gameObjectList.size(); ++it)
    {
		gameObjectList[it]->Start();
    }
}

void Scene::Update()
{
	vector<GameObject*> gameObjectList = m_psceneGraph->GetAllNodeObjects();
	for (int it = 0; it < gameObjectList.size(); ++it)
	{
		if(gameObjectList[it]->GetEnabled())
		{
			gameObjectList[it]->Update();
		}

	}
	gameObjectList = m_psceneGraph->GetAllNodeObjects();
	Collision::Update(gameObjectList);
}

void Scene::EditorUpdate()
{
	vector<GameObject*> gameObjectList = m_psceneGraph->GetAllNodeObjects();
	for (int it = 0; it < gameObjectList.size(); ++it)
	{
		gameObjectList[it]->EditorUpdate();
	}

	Collision::Update(gameObjectList);
}

void Scene::Render(RenderStruct& renderStruct)
{
	RenderableGameObject* prenderableGameObject = nullptr;

	vector<GameObject*> gameObjectList = m_psceneGraph->GetAllNodeObjects();
	for (int it = 0; it < gameObjectList.size(); ++it)
	{
		if (gameObjectList[it]->ContainsType(GameObjectType::RENDERABLE))
		{
			prenderableGameObject = dynamic_cast<RenderableGameObject*>(gameObjectList[it]);
			prenderableGameObject->Render(renderStruct);

			continue;
		}

		if (gameObjectList[it]->ContainsType(GameObjectType::GAME_UI_COMPONENT))
		{
			dynamic_cast<GameUIComponent*>(gameObjectList[it])->Render(renderStruct);
			continue;
		}
	}
}

vector<GameObject*>& Scene::GetAllGameObjects()
{
	return m_psceneGraph->GetAllNodeObjects();
}

CameraGameObject* Scene::GetActiveCamera()
{
	return m_pactiveCamera;
}

unordered_map<string, CameraGameObject*> Scene::GetCameraGameObjectMap()
{
	return m_cameraGameObjectMap;
}

bool Scene::SetActiveCameraUsingIdentifier(string identifier)
{
	if (m_cameraGameObjectMap.count(identifier) == 0)
	{
		return false;
	}
	m_pactiveCamera = m_cameraGameObjectMap[identifier];
}

void Scene::DeleteGameObjectUsingNode(TreeNode<GameObject>* currentNode)
{
	if (currentNode->NodeObject->ContainsType(GameObjectType::CAMERA))
	{
		m_cameraGameObjectMap.erase(currentNode->NodeObject->m_identifier);
		if (m_pactiveCamera == dynamic_cast<CameraGameObject*>(currentNode->NodeObject))
		{
			for (unordered_map<string, CameraGameObject*>::iterator it = m_cameraGameObjectMap.begin(); it != m_cameraGameObjectMap.end(); ++it)
			{
				m_pactiveCamera = it->second;
				return;
			}
			m_pactiveCamera = nullptr;
		}
	}

	m_psceneGraph->DeleteNodeObjectUsingNode(currentNode);
	
}

void Scene::DeleteGameObjectUsingIdentifier(string identifier)
{
	m_psceneGraph->DeleteNodeObjectUsingIdentifier(identifier);
}

TreeNode<GameObject>* Scene::GetRootTreeNode()
{
	return m_psceneGraph->GetRootNode();
}

TreeNode<GameObject>* Scene::GetTreeNode(GameObject* pgameObject)
{
	return m_psceneGraph->GetNodeUsingIdentifier(pgameObject->GetIdentifier());
}

string& Scene::GetSceneIdentifier()
{
	return m_sceneIdentifier;
}

GameObject* Scene::GetSelectedGameObject()
{
	return m_pselectedGameObject;
}

SceneGraph<GameObject>* Scene::GetSceneGraph()
{
	return m_psceneGraph;
}
