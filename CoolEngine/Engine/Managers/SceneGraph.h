#pragma once
#include "Engine/GameObjects/GameObject.h"

struct TreeNode
{
	TreeNode* Sibling = nullptr;
	TreeNode* Child = nullptr;
	TreeNode* PreviousNode = nullptr;
	GameObject* GameObject = nullptr;
};

class SceneGraph
{
private:
	TreeNode* m_rootNode;
	unordered_map<string, TreeNode*> m_sceneMap;

public:
	SceneGraph();
	~SceneGraph();

	TreeNode* NewNode(GameObject* gameObject);
	TreeNode* AddSibling(TreeNode* currentNode, GameObject* gameObject);
	TreeNode* AddChild(TreeNode* currentNode, GameObject* gameObject);
	TreeNode* TraverseTree(TreeNode* currentNode);
	void DeleteNode(TreeNode* currenNode);
	void DeleteGameObject(TreeNode* currenNode);

	TreeNode* GetRootNode();
	TreeNode* GetNodeUsingIdentifier(string identifier);
};

