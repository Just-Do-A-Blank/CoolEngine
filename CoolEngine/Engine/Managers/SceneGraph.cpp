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
		m_rootNode->Sibling = nullptr; 
		m_rootNode->Child = nullptr;
		m_rootNode->GameObject = gameObject;
		m_rootNode->PreviousParent = nullptr; 
		m_rootNode->PreviousSibling = nullptr;
		m_sceneMap.insert(pair<string, TreeNode*>(gameObject->GetIdentifier(), m_rootNode));
		return m_rootNode;		
	}

	TreeNode* newNode = new TreeNode();
	newNode->Sibling = nullptr; 
	newNode->Child = nullptr;
	newNode->GameObject = gameObject;
	newNode->PreviousParent = nullptr;
	newNode->PreviousSibling = nullptr;
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
		
	currentNode->Sibling = NewNode(gameObject);
	currentNode->Sibling->PreviousSibling = currentNode;

	return currentNode->Sibling;
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
		currentNode->Child = NewNode(gameObject);
		currentNode->Child->PreviousParent = currentNode;
		return currentNode->Child;
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

	delete currenNode->GameObject;
	currenNode->GameObject = nullptr;
	delete currenNode;
	currenNode = nullptr;
}

void SceneGraph::DeleteGameObject(TreeNode* currenNode)
{
	if (currenNode->Child)
	{
		DeleteNode(currenNode->Child);
	}

	if (currenNode->PreviousParent)
	{
		if (currenNode->Sibling)
		{
			currenNode->PreviousParent->Child = currenNode->Sibling;
			currenNode->Sibling->PreviousParent = currenNode->PreviousParent;
			currenNode->Sibling->PreviousSibling = nullptr;
		}
		else
		{
			currenNode->PreviousParent->Child = nullptr;
		}
	}
	else if (currenNode->PreviousSibling)
	{
		if (currenNode->Sibling)
		{
			currenNode->PreviousSibling->Sibling = currenNode->Sibling;
			currenNode->Sibling->PreviousSibling = currenNode->PreviousSibling;
			currenNode->Sibling->PreviousParent = nullptr;
		}
		else
		{
			currenNode->PreviousSibling->Sibling = nullptr;
		}
	}
	else
	{
		m_rootNode = currenNode->Sibling;
		if (currenNode->Sibling)
		{
			currenNode->Sibling->PreviousParent = nullptr;
			currenNode->Sibling->PreviousSibling = nullptr;
		}
	}

	delete currenNode->GameObject;
	currenNode->GameObject = nullptr;
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




