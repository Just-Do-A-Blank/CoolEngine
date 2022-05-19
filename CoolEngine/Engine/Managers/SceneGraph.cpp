#include "SceneGraph.h"
#include "Engine/GameObjects/Transform.h"
#include "Engine/GameUI/GameUIComponent.h"

#include <algorithm>

template<class T>
SceneGraph<T>::SceneGraph()
{

}

template<class T>
SceneGraph<T>::~SceneGraph()
{
	DeleteNode(m_rootNode);
}

template<class T>
TreeNode<T>* SceneGraph<T>::NewNode(T* gameObject)
{
	if (!m_rootNode)
	{
		m_rootNode = new TreeNode<T>;
		m_rootNode->Sibling = nullptr;
		m_rootNode->Child = nullptr;
		m_rootNode->NodeObject = gameObject;
		m_rootNode->PreviousParent = nullptr;
		m_rootNode->PreviousSibling = nullptr;
		m_sceneTreeNodeMap.insert(pair<string, TreeNode<T>*>(gameObject->GetIdentifier(), m_rootNode));
		m_sceneNodeObjectsMap.insert(pair<string, T*>(gameObject->GetIdentifier(), gameObject));
		m_sceneNodeObjectList.push_back(gameObject);
		m_sceneNodeList.push_back(m_rootNode);
		return m_rootNode;
	}

	TreeNode<T>* newNode = new TreeNode<T>();
	newNode->Sibling = nullptr;
	newNode->Child = nullptr;
	newNode->NodeObject = gameObject;
	newNode->PreviousParent = nullptr;
	newNode->PreviousSibling = nullptr;

	string gameObjectName = gameObject->GetIdentifier();
	m_sceneTreeNodeMap.insert(pair<string, TreeNode<T>*>(gameObjectName, newNode));
	m_sceneNodeObjectsMap.insert(pair<string, T*>(gameObjectName, gameObject));
	m_sceneNodeObjectList.push_back(gameObject);
	m_sceneNodeList.push_back(newNode);
	return newNode;
}

template<class T>
TreeNode<T>* SceneGraph<T>::AddSibling(TreeNode<T>* currentNode, T* gameObject)
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

	if (currentNode->PreviousParent != nullptr)
	{
		gameObject->GetTransform()->SetParentTransform(currentNode->PreviousParent->NodeObject->GetTransform());
		currentNode->PreviousParent->NodeObject->GetTransform()->AddChildTransform(gameObject->GetTransform());
	}

	return currentNode->Sibling;
}

template<class T>
void SceneGraph<T>::MoveNode(TreeNode<T>* currentNode, TreeNode<T>* parentNode)
{
	if (currentNode == nullptr)
	{
		return;
	}

	if (currentNode->Sibling)
	{
		currentNode->Sibling->PreviousSibling = currentNode->PreviousSibling;

	}
	if (currentNode == m_rootNode)
	{
		m_rootNode = currentNode->Sibling;
	}

	if (currentNode->PreviousSibling)
	{
		currentNode->PreviousSibling->Sibling = currentNode->Sibling;
	}
	else
	{
		if (currentNode->PreviousParent)
		{
			currentNode->PreviousParent->Child = currentNode->Sibling;
		}
	}

	if (currentNode->PreviousParent)
	{
		Transform* oldParentTransform = currentNode->PreviousParent->NodeObject->GetTransform();
		oldParentTransform->RemoveChildTransform(currentNode->NodeObject->GetTransform());
	}

	Transform* currentTransform = currentNode->NodeObject->GetTransform();

	currentNode->PreviousSibling = nullptr;
	currentNode->Sibling = nullptr;

	Transform* newParentTransform;
	if (parentNode)
	{
		if (!parentNode->Child)
		{
			parentNode->Child = currentNode;
			currentNode->PreviousParent = parentNode;
		}
		else
		{
			TreeNode<T>* siblingNode = parentNode->Sibling;
			while (siblingNode->Sibling != nullptr)
			{
				siblingNode = siblingNode->Sibling;
			}
			siblingNode->Sibling = currentNode;
			currentNode->PreviousSibling = siblingNode;
		}
		newParentTransform = parentNode->NodeObject->GetTransform();
		newParentTransform->AddChildTransform(currentTransform);
	}
	else
	{
		currentNode->PreviousParent = nullptr;

		TreeNode<T>* siblingNode = m_rootNode->Sibling;
		if (siblingNode == nullptr)
		{
			siblingNode = m_rootNode;
		}

		while (siblingNode->Sibling != nullptr)
		{
			siblingNode = siblingNode->Sibling;
		}

		siblingNode->Sibling = currentNode;
		currentNode->PreviousSibling = siblingNode;

		newParentTransform = nullptr;
	}

	currentTransform->SetParentTransform(newParentTransform);
}

template<class T>
TreeNode<T>* SceneGraph<T>::AddChild(TreeNode<T>* currentNode, T* gameObject)
{
	if (!currentNode)
	{
		return nullptr;
	}

	Transform* pparentTransform = currentNode->NodeObject->GetTransform();
	Transform* pcurrentTransform = gameObject->GetTransform();

	pcurrentTransform->SetParentTransform(pparentTransform);
	pparentTransform->AddChildTransform(pcurrentTransform);

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

template<class T>
TreeNode<T>* SceneGraph<T>::TraverseTree(TreeNode<T>* currentNode)
{
	if (!currentNode)
	{
		return nullptr;
	}

	while (currentNode)
	{
		LOG(currentNode->NodeObject->GetIdentifier());

		if (currentNode->Child)
		{
			TraverseTree(currentNode->Child);
		}
		currentNode = currentNode->Sibling;
	}
}

template<class T>
void SceneGraph<T>::DeleteNode(TreeNode<T>* currentNode, bool deleteData)
{
	if (currentNode->Child)
	{
		DeleteNode(currentNode->Child, deleteData);
	}
	if (currentNode->Sibling)
	{
		DeleteNode(currentNode->Sibling, deleteData);
	}

	if (currentNode->PreviousParent)
	{
		currentNode->PreviousParent->NodeObject->GetTransform()->RemoveChildTransform(currentNode->NodeObject->GetTransform());
	}

	string gameObjectName = currentNode->NodeObject->GetIdentifier();
	m_sceneTreeNodeMap.erase(gameObjectName);
	m_sceneNodeObjectsMap.erase(gameObjectName);
	m_sceneNodeObjectList.erase(remove(m_sceneNodeObjectList.begin(), m_sceneNodeObjectList.end(), currentNode->NodeObject));
	m_sceneNodeList.erase(remove(m_sceneNodeList.begin(), m_sceneNodeList.end(), currentNode));

	if (deleteData)
	{
		delete currentNode->NodeObject;
		currentNode->NodeObject = nullptr;
	}

	delete currentNode;
	currentNode = nullptr;
}

template<class T>
void SceneGraph<T>::DeleteNodeObjectUsingIdentifier(string identifier)
{
	unordered_map<string, TreeNode<T>*>::iterator it = m_sceneTreeNodeMap.find(identifier);
	if (it->second)
	{
		DeleteNodeObjectUsingNode(it->second);
	}
}

template<class T>
void SceneGraph<T>::DeleteNodeObjectUsingNode(TreeNode<T>* currentNode)
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
	m_sceneNodeObjectList.erase(remove(m_sceneNodeObjectList.begin(), m_sceneNodeObjectList.end(), currentNode->NodeObject));
	m_sceneNodeList.erase(remove(m_sceneNodeList.begin(), m_sceneNodeList.end(), currentNode));

	delete currentNode->NodeObject;
	currentNode->NodeObject = nullptr;
	delete currentNode;
	currentNode = nullptr;
}

template<class T>
TreeNode<T>* SceneGraph<T>::GetRootNode()
{
	return m_rootNode;
}

template<class T>
TreeNode<T>* SceneGraph<T>::GetNodeUsingIdentifier(string identifier)
{
	unordered_map<string, TreeNode<T>*>::iterator it;
	it = m_sceneTreeNodeMap.find(identifier);

	if (it == m_sceneTreeNodeMap.end() || !it->second)
	{
		return nullptr;
	}
	return it->second;
}

template<class T>
vector<T*>& SceneGraph<T>::GetAllNodeObjects()
{
	return m_sceneNodeObjectList;
}

template<class T>
vector<TreeNode<T>*>& SceneGraph<T>::GetAllNodes()
{
	return m_sceneNodeList;
}

template<class T>
T* SceneGraph<T>::GetNodeObjectUsingIdentifier(string& identifier)
{
	if (m_sceneNodeObjectsMap.count(identifier) == 0)
	{
		return nullptr;
	}
	
	return m_sceneNodeObjectsMap[identifier];
}
