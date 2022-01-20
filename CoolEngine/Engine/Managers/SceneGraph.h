#pragma once
#include "Engine/GameObjects/GameObject.h"

struct TreeNode
{
	TreeNode* Sibling = nullptr;
	TreeNode* Child = nullptr;
	TreeNode* PreviousParent = nullptr;
	TreeNode* PreviousSibling = nullptr;
	GameObject* GameObject = nullptr;
};

class SceneGraph
{
private:
	TreeNode* m_rootNode;
	unordered_map<string, TreeNode*> m_sceneTreeNodeMap;
	unordered_map<string, GameObject*> m_sceneGameObjectsMap;

public:
	SceneGraph();
	~SceneGraph();

	TreeNode* NewNode(GameObject* gameObject);
	TreeNode* AddSibling(TreeNode* currentNode, GameObject* gameObject);
	TreeNode* AddChild(TreeNode* currentNode, GameObject* gameObject);
	TreeNode* TraverseTree(TreeNode* currentNode);
	void DeleteNode(TreeNode* currenNode);

	void DeleteGameObjectUsingIdentifier(string identifier);
	void DeleteGameObjectUsingNode(TreeNode* currenNode);

	TreeNode* GetRootNode();
	TreeNode* GetNodeUsingIdentifier(string identifier);

	//Getters
	unordered_map<string, GameObject*>& GetAllGameObjects();
	GameObject* GetGameObjectUsingIdentifier(string& identifier);
};

