#pragma once
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/Managers/SceneGraph.h"
#include "Engine/GameObjects/CameraGameObject.h"

class GameObject;

class Scene
{
	friend class GameManager;
private:
	string m_sceneIdentifier;
	SceneGraph<GameObject>* m_psceneGraph;

	GameObject* m_pselectedGameObject = nullptr;
	TreeNode<GameObject>* m_prootTreeNode = nullptr;
	unordered_map<string, CameraGameObject*> m_cameraGameObjectMap;
	CameraGameObject* m_pactiveCamera = nullptr;
	
public:
	Scene(string identifier);
	~Scene();

	virtual void Start();
	virtual void Update();
	virtual void EditorUpdate();
	virtual void Render(RenderStruct& renderStruct);

	SceneGraph<GameObject>* GetSceneGraph();

private:

	vector<GameObject*>& GetAllGameObjects();
	CameraGameObject* GetActiveCamera();

	unordered_map<string, CameraGameObject*> GetCameraGameObjectMap();
	bool SetActiveCameraUsingIdentifier(string identifier);

	template<typename T>
	T* GetGameObjectUsingIdentifier(string& identifier)
	{
		GameObject* pobject = m_psceneGraph->GetNodeObjectUsingIdentifier(identifier);

		if (pobject == nullptr)
		{
			return nullptr;
		}
		else
		{
			return  dynamic_cast<T*>(pobject);
		}

	}

	template<typename T>
	T* CreateGameObject(string identifier, TreeNode<GameObject>* nodeParent = nullptr)
	{
		assert(m_psceneGraph != nullptr);
		
		CoolUUID uuid;
		T* gameObject = new T(std::string(identifier), uuid);

		if (std::is_same<T, CameraGameObject>::value)
		{
			CameraGameObject* cameraObject = dynamic_cast<CameraGameObject*>(gameObject);
			m_cameraGameObjectMap[identifier] = cameraObject;
			if (!m_pactiveCamera)
			{
				m_pactiveCamera = cameraObject;
			}
		}

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

		if (std::is_same<T, CameraGameObject>::value)
		{
			CameraGameObject* cameraObject = dynamic_cast<CameraGameObject*>(gameObject);
			m_cameraGameObjectMap[pgameObject->m_identifier] = cameraObject;
			if (!m_pactiveCamera)
			{
				m_pactiveCamera = cameraObject;
			}
		}

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

