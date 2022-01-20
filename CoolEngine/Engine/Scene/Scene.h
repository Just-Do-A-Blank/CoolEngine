#pragma once
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/Managers/SceneGraph.h"

class SceneGraph;
class GameObject;
class TreeNode;

class Scene
{
	friend class GameManager;
private:
	string m_sceneIdentifier;
	SceneGraph* m_psceneGraph;

	GameObject* m_pselectedGameObject = nullptr;
	TreeNode* m_pselectedNode = nullptr;
	TreeNode* m_prootTreeNode = nullptr;
	
public:
	Scene(string identifier);
	~Scene();

	virtual void Update();
	virtual void Render(RenderStruct& renderStruct);

private:

	unordered_map<string, GameObject*>& GetAllGameObjects();

	template<typename T>
	T* GetGameObjectUsingIdentifier(string& identifier)
	{
		return  static_cast<T*>(m_psceneGraph->GetGameObjectUsingIdentifier(identifier));
	}

	void SelectGameObjectUsingIdentifier(string identifier);
	void SelectGameObject(GameObject* pgameObject);
	void SelectGameObjectUsingTreeNode(TreeNode* pnode);


	template<typename T>
	T* CreateGameObject(string identifier)
	{
		T* gameObject = new T(identifier);

		m_prootTreeNode = m_psceneGraph->GetRootNode();
		if (!m_prootTreeNode)
		{
			m_prootTreeNode = m_psceneGraph->NewNode(gameObject);
		}
		else
		{
			if (!m_pselectedGameObject)
			{
				m_psceneGraph->AddSibling(m_prootTreeNode, gameObject);
			}
			else
			{
				m_psceneGraph->AddChild(m_psceneGraph->GetNodeUsingIdentifier(m_pselectedGameObject->GetIdentifier()), gameObject);
			}
		}

		return gameObject;
	}


	void DeleteSelectedGameObject();
	void DeleteGameObjectUsingIdentifier(string identifier);

	//Getters
	TreeNode* GetRootTreeNode();
	TreeNode* GetTreeNode(GameObject* pgameObject);
	string& GetSceneIdentifier();
	GameObject* GetSelectedGameObject();
};

