#pragma once
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/Managers/SceneGraph.h"

class GameObject;

class Scene
{
	friend class GameManager;
private:
	string m_sceneIdentifier;
	SceneGraph<GameObject>* m_psceneGraph;

	GameObject* m_pselectedGameObject = nullptr;
	TreeNode<GameObject>* m_pselectedNode = nullptr;
	TreeNode<GameObject>* m_prootTreeNode = nullptr;
	
public:
	Scene(string identifier);
	~Scene();

	virtual void Update();
	virtual void Render(RenderStruct& renderStruct);

	SceneGraph<GameObject>* GetSceneGraph();

private:

	vector<GameObject*>& GetAllGameObjects();

	template<typename T>
	T* GetGameObjectUsingIdentifier(string& identifier)
	{
		return  dynamic_cast<T*>(m_psceneGraph->GetGameObjectUsingIdentifier(identifier));
	}

	void SelectGameObjectUsingIdentifier(string identifier);
	void SelectGameObject(GameObject* pgameObject);
	void SelectGameObjectUsingTreeNode(TreeNode<GameObject>* pnode);


	template<typename T>
	T* CreateGameObject(string identifier)
	{
		assert(m_psceneGraph != nullptr);

		CoolUUID uuid;
		T* gameObject = new T(identifier, uuid);

		GameObject* pgameObject = dynamic_cast<GameObject*>(gameObject);
		pgameObject->m_identifier = identifier;

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

	template<typename T>
	T* CreateGameObject(string identifier, CoolUUID uuid)
	{
		assert(m_psceneGraph != nullptr);

		T* gameObject = new T(identifier, uuid);

		GameObject* pgameObject = dynamic_cast<GameObject*>(gameObject);
		pgameObject->m_identifier = identifier;

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

	template<typename T>
	void DeleteGameObject(T* pgameObject, std::string identifier)
	{
		m_psceneGraph->DeleteGameObject(pgameObject, identifier);
	}

	//Getters
	TreeNode<GameObject>* GetRootTreeNode();
	TreeNode<GameObject>* GetTreeNode(GameObject* pgameObject);
	string& GetSceneIdentifier();
	GameObject* GetSelectedGameObject();
	
};

