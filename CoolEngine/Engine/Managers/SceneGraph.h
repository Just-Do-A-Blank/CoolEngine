#pragma once
#include "Engine/GameObjects/GameObject.h"

class GameUIComponent;

template<class T>
struct TreeNode
{
	TreeNode* Sibling = nullptr;
	TreeNode* Child = nullptr;
	TreeNode* PreviousParent = nullptr;
	TreeNode* PreviousSibling = nullptr;
	T* NodeObject = nullptr;
};

template<class T>
class SceneGraph
{
private:
	TreeNode<T>* m_rootNode = nullptr;
	unordered_map<string, TreeNode<T>*> m_sceneTreeNodeMap;
	unordered_map<string, T*> m_sceneNodeObjectsMap;
	unordered_map<UINT64, T*> m_sceneNodeObjectsUUIDMap;
	vector<T*> m_sceneNodeObjectList;
	vector<TreeNode<T>*> m_sceneNodeList;

public:
	SceneGraph();
	~SceneGraph();

	TreeNode<T>* NewNode(T* gameObject);
	TreeNode<T>* AddSibling(TreeNode<T>* currentNode, T* gameObject);
	void MoveNode(TreeNode<T>* currentNode, TreeNode<T>* parentNode);
	TreeNode<T>* AddChild(TreeNode<T>* currentNode, T* gameObject);
	TreeNode<T>* TraverseTree(TreeNode<T>* currentNode);
	void DeleteNode(TreeNode<T>* currenNode, bool deleteData = true);

	void DeleteNodeObjectUsingIdentifier(string identifier);
	void DeleteNodeObjectUsingNode(TreeNode<T>* currenNode);
	void DeleteAllGameObjects();

	template <typename K>
	void DeleteNodeObjectUsingNode(TreeNode<T>* currentNode, K* pgameObject)
	{
		if (currentNode->Child)
		{
			DeleteNode(currentNode->Child);
		}

		if (currentNode->PreviousParent)
		{
			if (currentNode->Sibling)
			{
				currentNode->PreviousParent->Child = currentNode->Sibling;
				currentNode->Sibling->PreviousParent = currentNode->PreviousParent;
				currentNode->Sibling->PreviousSibling = nullptr;
			}
			else
			{
				currentNode->PreviousParent->Child = nullptr;
			}
		}
		else if (currentNode->PreviousSibling)
		{
			if (currentNode->Sibling)
			{
				currentNode->PreviousSibling->Sibling = currentNode->Sibling;
				currentNode->Sibling->PreviousSibling = currentNode->PreviousSibling;
				currentNode->Sibling->PreviousParent = nullptr;
			}
			else
			{
				currentNode->PreviousSibling->Sibling = nullptr;
			}
		}
		else
		{
			m_rootNode = currentNode->Sibling;
			if (currentNode->Sibling)
			{
				currentNode->Sibling->PreviousParent = nullptr;
				currentNode->Sibling->PreviousSibling = nullptr;
			}
		}

		string gameObjectName = currentNode->NodeObject->GetIdentifier();
		m_sceneTreeNodeMap.erase(gameObjectName);
		m_sceneNodeObjectsMap.erase(gameObjectName);
		m_sceneNodeObjectList.erase(std::find(m_sceneNodeObjectList.begin(), m_sceneNodeObjectList.end(), currentNode->NodeObject));

		delete pgameObject;
		currentNode->NodeObject = nullptr;
		delete currentNode;
		currentNode = nullptr;
	}

	template<typename K>
	void DeleteGameObject(K* pgameObject, std::string identifier)
	{
		unordered_map<string, TreeNode<T>*>::iterator it = m_sceneTreeNodeMap.find(identifier);
		if (it->second)
		{
			DeleteNodeObjectUsingNode(it->second, pgameObject);
		}
	}

	TreeNode<T>* GetRootNode();
	TreeNode<T>* GetNodeUsingIdentifier(string identifier);

	//Getters
	vector<T*>& GetAllNodeObjects();
	vector<TreeNode<T>*>& GetAllNodes();
	T* GetNodeObjectUsingIdentifier(string& identifier);
	T* GetNodeObjectUsingUUID(CoolUUID& uuid);
};
template class SceneGraph<GameObject>;
template class SceneGraph<GameUIComponent>;
