#include "Scene.h"

#include "Engine/GameObjects/RenderableGameObject.h"
#include "Engine/Managers/SceneGraph.h"

#include "Engine/Physics/Collision.h"

Scene::Scene(string identifier)
{
	m_sceneIdentifier = identifier;
	m_psceneGraph = new SceneGraph<GameObject>();
}

Scene::~Scene()
{
}

void Scene::Update()
{
	vector<GameObject*> gameObjectList = m_psceneGraph->GetAllGameObjects();
	for (int it = 0; it < gameObjectList.size(); ++it)
	{
		gameObjectList[it]->Update();
	}

	Collision::Update(gameObjectList);
}

void Scene::Render(RenderStruct& renderStruct)
{
	RenderableGameObject* prenderableGameObject = nullptr;

	for (int i = 0; i < GraphicsManager::GetInstance()->GetNumLayers(); ++i)
	{
		vector<GameObject*> gameObjectList = m_psceneGraph->GetAllGameObjects();
		for (int it = 0; it < gameObjectList.size(); ++it)
		{
			if (gameObjectList[it]->ContainsType(GameObjectType::RENDERABLE) == false)
			{
				continue;
			}

			prenderableGameObject = dynamic_cast<RenderableGameObject*>(gameObjectList[it]);

			if (prenderableGameObject->IsRenderable() == false || prenderableGameObject->GetLayer() != i)
			{
				continue;
			}

			prenderableGameObject->Render(renderStruct);
		}
	}
}

vector<GameObject*>& Scene::GetAllGameObjects()
{
	return m_psceneGraph->GetAllGameObjects();
}

void Scene::SelectGameObjectUsingIdentifier(string identifier)
{
	m_pselectedNode = m_psceneGraph->GetNodeUsingIdentifier(identifier);
	m_pselectedGameObject = m_pselectedNode->GameObject;
}

void Scene::SelectGameObject(GameObject* pgameObject)
{
	if (pgameObject)
	{
		SelectGameObjectUsingIdentifier(pgameObject->GetIdentifier());
	}
	else
	{
		m_pselectedNode = nullptr;
		m_pselectedGameObject = nullptr;
	}
}

void Scene::SelectGameObjectUsingTreeNode(TreeNode<GameObject>* pnode)
{
	if (!pnode)
	{
		m_pselectedNode = nullptr;
		m_pselectedGameObject = nullptr;
		return;
	}

	m_pselectedNode = pnode;
	m_pselectedGameObject = pnode->GameObject;
}


void Scene::DeleteSelectedGameObject()
{
	if (!m_pselectedNode)
	{
		return;
	}
	m_psceneGraph->DeleteGameObjectUsingNode(m_pselectedNode);
}

void Scene::DeleteGameObjectUsingIdentifier(string identifier)
{
	m_psceneGraph->DeleteGameObjectUsingIdentifier(identifier);
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
