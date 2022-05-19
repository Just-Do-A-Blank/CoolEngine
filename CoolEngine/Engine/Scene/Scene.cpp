#include "Scene.h"
#include "Engine/GameObjects/RenderableGameObject.h"
#include "Engine/Managers/SceneGraph.h"
#include "Engine/Physics/Collision.h"
#include "Engine/GameUI/GameUIComponent.h"

Scene::Scene(string identifier)
{
	m_sceneIdentifier = identifier;
	m_psceneGraph = new SceneGraph<GameObject>();
	
	m_tree = new Quadtree(0,0,0,0, nullptr);

}

Scene::~Scene()
{

}

void Scene::Update()
{
	vector<GameObject*> gameObjectList = m_psceneGraph->GetAllNodeObjects();

	if (gameObjectList.size() != m_treeSize)
	{
		for (size_t i = 0; i < gameObjectList.size(); i++)
		{
			if (gameObjectList[i]->ContainsType(GameObjectType::COLLIDABLE) && gameObjectList[i]->ContainsType(GameObjectType::RENDERABLE))
			{
				XMFLOAT3 pos = gameObjectList[i]->GetTransform()->GetWorldPosition();
				XMFLOAT3 scale = gameObjectList[i]->GetTransform()->GetWorldScale(); 
				

				m_tree->insert(pos.x, pos.y, (scale.y * 2.0f), (scale.x * 2.0f), gameObjectList[i]);
				++m_treeSize;
				m_tree->
			}
			else
			{
				++m_treeSize;
			}
		}
	}

	for (int it = 0; it < gameObjectList.size(); ++it)
	{
		gameObjectList[it]->Update();
	}

 //			GameObject* node = m_tree->search(pos.x, pos.y)->get_data();


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

void Scene::DeleteGameObjectUsingNode(TreeNode<GameObject>* currentNode)
{
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
