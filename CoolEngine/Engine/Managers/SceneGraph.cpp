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
		m_sceneTreeNodeMap.insert(pair<string, TreeNode*>(gameObject->GetIdentifier(), m_rootNode));
		m_sceneGameObjectsMap.insert(pair<string, GameObject*>(gameObject->GetIdentifier(), gameObject));
		return m_rootNode;		
	}

	TreeNode* newNode = new TreeNode();
	newNode->Sibling = nullptr; 
	newNode->Child = nullptr;
	newNode->GameObject = gameObject;
	newNode->PreviousParent = nullptr;
	newNode->PreviousSibling = nullptr;

	string gameObjectName = gameObject->GetIdentifier();
	m_sceneTreeNodeMap.insert(pair<string, TreeNode*>(gameObjectName, newNode));
	m_sceneGameObjectsMap.insert(pair<string, GameObject*>(gameObjectName, gameObject));
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

void SceneGraph::DeleteNode(TreeNode* currentNode)
{
	if (currentNode->Child)
	{
		DeleteNode(currentNode->Child);
	}
	if (currentNode->Sibling)
	{
		DeleteNode(currentNode->Sibling);
	}

	delete currentNode->GameObject;
	currentNode->GameObject = nullptr;
	delete currentNode;
	currentNode = nullptr;

	string gameObjectName = currentNode->GameObject->GetIdentifier();
	m_sceneTreeNodeMap.erase(gameObjectName);
	m_sceneGameObjectsMap.erase(gameObjectName);
}

void SceneGraph::DeleteGameObjectUsingIdentifier(string identifier)
{
	unordered_map<string, TreeNode*>::iterator it = m_sceneTreeNodeMap.find(identifier);
	if (it->second)
	{
		DeleteGameObjectUsingNode(it->second);
	}
}

void SceneGraph::DeleteGameObjectUsingNode(TreeNode* currentNode)
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

	string gameObjectName = currentNode->GameObject->GetIdentifier();
	m_sceneTreeNodeMap.erase(gameObjectName);
	m_sceneGameObjectsMap.erase(gameObjectName);

	delete currentNode->GameObject;
	currentNode->GameObject = nullptr;
	delete currentNode;
	currentNode = nullptr;	
}


TreeNode* SceneGraph::GetRootNode()
{
	return m_rootNode;
}

TreeNode* SceneGraph::GetNodeUsingIdentifier(string identifier)
{
	unordered_map<string, TreeNode*>::iterator it;
	it = m_sceneTreeNodeMap.find(identifier);

	if (!it->second)
	{
		return nullptr;
	}
	return it->second;
}

unordered_map<string, GameObject*>& SceneGraph::GetAllGameObjects()
{
	return m_sceneGameObjectsMap;
}

GameObject* SceneGraph::GetGameObjectUsingIdentifier(string& identifier)
{
	unordered_map<string, GameObject*>::iterator it = m_sceneGameObjectsMap.find(identifier);
	if (!it->second)
	{
		return nullptr;
	}
	return it->second;
}




