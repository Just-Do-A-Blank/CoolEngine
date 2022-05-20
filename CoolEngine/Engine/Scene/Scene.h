#pragma once
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/Managers/SceneGraph.h"
#include "Engine/Includes/Quad Tree/include/quadtree.h"

class GameObject;

struct Node
{
	GameObject* m_gameObject;
	GameObjectType m_type;
	CoolUUID m_id;
};


class Scene
{
	friend class GameManager;
private:
	string m_sceneIdentifier;
	SceneGraph<GameObject>* m_psceneGraph;

	GameObject* m_pselectedGameObject = nullptr;
	TreeNode<GameObject>* m_prootTreeNode = nullptr;
	
	Quadtree* m_quadtree = nullptr;

	int m_treeSizeOffset = 0;

public:
	Scene(string identifier);
	~Scene();

	virtual void Update();
	virtual void EditorUpdate();
	virtual void Render(RenderStruct& renderStruct);

	SceneGraph<GameObject>* GetSceneGraph();

	void InitializeQuadTree();

private:

	vector<GameObject*>& GetAllGameObjects();

	template<typename T>
	T* GetGameObjectUsingIdentifier(string& identifier)
	{
		return  dynamic_cast<T*>(m_psceneGraph->GetNodeObjectUsingIdentifier(identifier));
	}

	template<typename T>
	T* CreateGameObject(string identifier, TreeNode<GameObject>* nodeParent = nullptr)
	{
		assert(m_psceneGraph != nullptr);

		CoolUUID uuid;
		T* gameObject = new T(std::string(identifier), uuid);

		GameObject* pgameObject = dynamic_cast<GameObject*>(gameObject);
		pgameObject->m_identifier = identifier;

		m_prootTreeNode = m_psceneGraph->GetRootNode();
		if (!m_prootTreeNode)
		{
			m_prootTreeNode = m_psceneGraph->NewNode(gameObject);
		}
		else
		{
			if (nodeParent == nullptr)
			{
				m_psceneGraph->AddSibling(m_prootTreeNode, gameObject);
			}
			else
			{
				m_psceneGraph->AddChild(nodeParent, gameObject);
			}
		}

		return gameObject;
	}

	template<typename T>
	T* CopyGameObject(T object, TreeNode<GameObject>* nodeParent = nullptr, TreeNode<GameObject>* nodePreviousSibling = nullptr)
	{
		assert(m_psceneGraph != nullptr);

		CoolUUID uuid;
		T* gameObject = new T(object);

		GameObject* pgameObject = dynamic_cast<GameObject*>(gameObject);

		m_prootTreeNode = m_psceneGraph->GetRootNode();
		if (!m_prootTreeNode)
		{
			m_prootTreeNode = m_psceneGraph->NewNode(gameObject);
		}
		else
		{
			if (nodeParent == nullptr)
			{
				if (nodePreviousSibling == nullptr)
				{
					m_psceneGraph->AddSibling(m_prootTreeNode, gameObject);
				}
				else
				{
					m_psceneGraph->AddSibling(nodePreviousSibling, gameObject);
				}
			}
			else
			{
				m_psceneGraph->AddChild(nodeParent, gameObject);
			}
		}

		return gameObject;
	}

	template<typename T>
	T* CreateGameObject(string identifier, CoolUUID uuid, TreeNode<GameObject>* nodeParent = nullptr)
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
			if (nodeParent == nullptr)
			{
				m_psceneGraph->AddSibling(m_prootTreeNode, gameObject);
			}
			else
			{
				m_psceneGraph->AddChild(nodeParent, gameObject);
			}
		}

		return gameObject;
	}

	void DeleteGameObjectUsingNode(TreeNode<GameObject>* currentNode);
	void DeleteGameObjectUsingIdentifier(string identifier);

	template<typename T>
	void DeleteGameObjectUsingNode(T* pgameObject, std::string identifier)
	{
		m_psceneGraph->DeleteGameObject(pgameObject, identifier);
	}

	//Getters
	TreeNode<GameObject>* GetRootTreeNode();
	TreeNode<GameObject>* GetTreeNode(GameObject* pgameObject);
	string& GetSceneIdentifier();
	GameObject* GetSelectedGameObject();
	
};

