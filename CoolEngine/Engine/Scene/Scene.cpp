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

	vector<GameObject*> gameObjectList = m_psceneGraph->GetAllGameObjects();
	for (int it = 0; it < gameObjectList.size(); ++it)
	{
		if (gameObjectList[it]->ContainsType(GameObjectType::RENDERABLE) == false)
		{
			continue;
		}

		prenderableGameObject = dynamic_cast<RenderableGameObject*>(gameObjectList[it]);

		prenderableGameObject->Render(renderStruct);
	}
}

vector<GameObject*>& Scene::GetAllGameObjects()
{
	return m_psceneGraph->GetAllGameObjects();
}

void Scene::DeleteGameObjectUsingNode(TreeNode<GameObject>* currentNode)
{
	m_psceneGraph->DeleteGameObjectUsingNode(currentNode);
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

SceneGraph<GameObject>* Scene::GetSceneGraph()
{
	return m_psceneGraph;
}
