#include "SceneGraph.h"

SceneGraph::SceneGraph()
{
	
}

SceneGraph::~SceneGraph()
{
	DeleteNode(m_rootNode);
}

TreeNode* SceneGraph::NewNode(GameObject* gameObject)
{
	if (!m_rootNode)
	{
		m_rootNode = new TreeNode;
		m_rootNode->Sibling = m_rootNode->Child = nullptr;
		m_rootNode->GameObject = gameObject;

		m_sceneMap.insert(pair<string, TreeNode*>(gameObject->GetIdentifier(), m_rootNode));
		return m_rootNode;		
	}

	TreeNode* newNode = new TreeNode();
	newNode->Sibling = newNode->Child = nullptr;
	newNode->GameObject = gameObject;

	m_sceneMap.insert(pair<string, TreeNode*>(gameObject->GetIdentifier(), newNode));
	return newNode;
}

TreeNode* SceneGraph::AddSibling(TreeNode* currentNode, GameObject* gameObject)
{
	if (!currentNode)
	{
		return nullptr;
	}

	while (currentNode->Sibling)
	{
		currentNode = currentNode->Sibling;
	}

	return currentNode->Sibling = NewNode(gameObject);
}

TreeNode* SceneGraph::AddChild(TreeNode* currentNode, GameObject* gameObject)
{
	if (!currentNode)
	{
		return nullptr;
	}

	if(currentNode->Child)
	{
		return AddSibling(currentNode->Child, gameObject);
	}
	else
	{
		return currentNode->Child = NewNode(gameObject);
	}
}

TreeNode* SceneGraph::TraverseTree(TreeNode* currentNode)
{
	if (!currentNode)
	{
		return nullptr;
	}

	while (currentNode)
	{
		LOG(currentNode->GameObject->GetIdentifier());

		if (currentNode->Child)
		{
			TraverseTree(currentNode->Child);
		}
		currentNode = currentNode->Sibling;
	}
}

void SceneGraph::DeleteNode(TreeNode* currenNode)
{
	if (currenNode->Child)
	{
		DeleteNode(currenNode->Child);
	}
	if (currenNode->Sibling)
	{
		DeleteNode(currenNode->Sibling);
	}

	delete currenNode;
	currenNode = nullptr;
}

TreeNode* SceneGraph::GetRootNode()
{
	return m_rootNode;
}

TreeNode* SceneGraph::GetNodeUsingIdentifier(string identifier)
{
	unordered_map<string, TreeNode*>::iterator iterator;

	iterator = m_sceneMap.find(identifier);

	return iterator->second;
}




