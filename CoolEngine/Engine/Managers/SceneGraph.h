#pragma once
#include "Engine/GameObjects/GameObject.h"

struct TreeNode
{
	TreeNode* Sibling;
	TreeNode* Child;
	GameObject* GameObject;
};

class SceneGraph
{
private:
	TreeNode* m_rootNode;
	map<string, TreeNode*> m_sceneMap;

public:
	SceneGraph();
	~SceneGraph();

	TreeNode* NewNode(GameObject* gameObject);
	TreeNode* AddSibling(TreeNode* currentNode, GameObject* gameObject);
	TreeNode* AddChild(TreeNode* currentNode, GameObject* gameObject);
	TreeNode* TraverseTree(TreeNode* currentNode);
	void DeleteNode(TreeNode* currenNode);

	TreeNode* GetRootNode();
	TreeNode* GetNodeUsingIdentifier(string identifier);
};

