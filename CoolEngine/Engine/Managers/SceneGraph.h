#pragma once
#include "Engine/GameObjects/GameObject.h"

class UiElement;

template<class T>
struct TreeNode
{
	TreeNode* Sibling = nullptr;
	TreeNode* Child = nullptr;
	TreeNode* PreviousParent = nullptr;
	TreeNode* PreviousSibling = nullptr;
	T* GameObject = nullptr;
};

template<class T>
class SceneGraph
{
private:
	TreeNode<T>* m_rootNode = nullptr;
	unordered_map<string, TreeNode<T>*> m_sceneTreeNodeMap;
	unordered_map<string, T*> m_sceneGameObjectsMap;

public:
	SceneGraph();
	~SceneGraph();

	TreeNode<T>* NewNode(T* gameObject);
	TreeNode<T>* AddSibling(TreeNode<T>* currentNode, T* gameObject);
	TreeNode<T>* AddChild(TreeNode<T>* currentNode, T* gameObject);
	TreeNode<T>* TraverseTree(TreeNode<T>* currentNode);
	void DeleteNode(TreeNode<T>* currenNode);

	void DeleteGameObjectUsingIdentifier(string identifier);
	void DeleteGameObjectUsingNode(TreeNode<T>* currenNode);

	TreeNode<T>* GetRootNode();
	TreeNode<T>* GetNodeUsingIdentifier(string identifier);

	//Getters
	unordered_map<string, T*>& GetAllGameObjects();
	T* GetGameObjectUsingIdentifier(string& identifier);
};
template class SceneGraph<GameObject>;
template class SceneGraph<UiElement>;
