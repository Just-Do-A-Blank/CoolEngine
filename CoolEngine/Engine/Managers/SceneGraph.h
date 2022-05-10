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
	unordered_map<string, T*> m_sceneGameObjectsMap;
	vector<T*> m_sceneGameObjectList;

public:
	SceneGraph();
	~SceneGraph();

	TreeNode<T>* NewNode(T* gameObject);
	TreeNode<T>* AddSibling(TreeNode<T>* currentNode, T* gameObject);
	void MoveNode(TreeNode<T>* currentNode, TreeNode<T>* parentNode);
	TreeNode<T>* AddChild(TreeNode<T>* currentNode, T* gameObject);
	TreeNode<T>* TraverseTree(TreeNode<T>* currentNode);
	void DeleteNode(TreeNode<T>* currenNode, bool deleteData = true);

	void DeleteGameObjectUsingIdentifier(string identifier);
	void DeleteGameObjectUsingNode(TreeNode<T>* currenNode);

	template <typename K>
	void DeleteGameObjectUsingNode(TreeNode<T>* currentNode, K* pgameObject)
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
		m_sceneGameObjectsMap.erase(gameObjectName);
		m_sceneGameObjectList.erase(std::find(m_sceneGameObjectList.begin(), m_sceneGameObjectList.end(), currentNode->NodeObject));

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
			DeleteGameObjectUsingNode(it->second, pgameObject);
		}
	}

	TreeNode<T>* GetRootNode();
	TreeNode<T>* GetNodeUsingIdentifier(string identifier);

	//Getters
	vector<T*>& GetAllGameObjects();
	T* GetGameObjectUsingIdentifier(string& identifier);
};
template class SceneGraph<GameObject>;
template class SceneGraph<GameUIComponent>;
