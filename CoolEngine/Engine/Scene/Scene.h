#pragma once
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/GameObjects/PlayerGameObject.h"

class SceneGraph;
class GameObject;
class TreeNode;

class Scene
{
private:
	string m_sceneIdentifier;
	SceneGraph* m_psceneGraph;

	GameObject* m_pcurrentlySelectedGameObject = nullptr;
	TreeNode* m_prootTreeNode = nullptr;
	
public:
	Scene(string identifier);
	~Scene();

	virtual void Update();
	virtual void Render(RenderStruct& renderStruct);

	unordered_map<string, GameObject*>& GetAllGameObjects();
	GameObject* GetGameObjectUsingIdentifier(string& identifier);
	PlayerGameObject* GetPlayerGameObjectUsingIdentifier(string& identifier);
	void SelectGameObjectUsingIdentifier(string& identifier);
	void SelectGameObject(GameObject* pgameObject);

	GameObject* CreateGameObject(string identifier);
	GameObject* CreateGameObject(string identifier, TreeNode* pparentNode);
	GameObject* CreatePlayerGameObject(string identifier);
	GameObject* CreatePlayerGameObject(string identifier, TreeNode* pparentNode);

	void DeleteGameObjectUsingNode(TreeNode* pnode);
	void DeleteGameObjectUsingIdentifier(string identifier);
	void DeleteSelectedGameObject();

	//Getters
	TreeNode* GetRootTreeNode();
	TreeNode* GetTreeNode(GameObject* pgameObject);
	string& GetSceneName();
};

