#pragma once
#include "Engine/Managers/GraphicsManager.h"
#include "Engine/GameObjects/PlayerGameObject.h"

class SceneGraph;
class GameObject;
class TreeNode;

class Scene
{
	//friend class GameManager;
private:
	string m_sceneIdentifier;
	SceneGraph* m_psceneGraph;

	GameObject* m_pselectedGameObject = nullptr;
	TreeNode* m_pselectedNode = nullptr;
	TreeNode* m_prootTreeNode = nullptr;
	
public:
	Scene(string identifier);
	~Scene();

	virtual void Update();
	virtual void Render(RenderStruct& renderStruct);

//private:

	unordered_map<string, GameObject*>& GetAllGameObjects();
	GameObject* GetGameObjectUsingIdentifier(string& identifier);
	void SelectGameObjectUsingIdentifier(string identifier);
	void SelectGameObject(GameObject* pgameObject);
	void SelectGameObjectUsingTreeNode(TreeNode* pnode);

	PlayerGameObject* GetPlayerGameObjectUsingIdentifier(string& identifier);

	GameObject* CreateGameObject(string identifier);
	GameObject* CreateGameObject(string identifier, TreeNode* pparentNode);
	GameObject* CreatePlayerGameObject(string identifier);
	GameObject* CreatePlayerGameObject(string identifier, TreeNode* pparentNode);

	void DeleteSelectedGameObject();
	void DeleteGameObjectUsingIdentifier(string identifier);

	//Getters
	TreeNode* GetRootTreeNode();
	TreeNode* GetTreeNode(GameObject* pgameObject);
	string& GetSceneIdentifier();
	GameObject* GetSelectedGameObject();
};

