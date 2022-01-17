#pragma once
#include "Engine/Managers/GraphicsManager.h"

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
	unordered_map<string, GameObject*> m_gameObjectMap;
	
public:
	Scene(string identifier);
	~Scene();

	virtual void Update();
	//virtual void Render(RenderStruct& renderStruct);

	unordered_map<string, GameObject*>& GetAllGameObjects();
	GameObject* GetGameObjectUsingIdentifier(string& identifier);
	void SelectGameObjectUsingIdentifier(string& identifier);

	GameObject* CreateGameObject(string identifier, GameObject* pparentObject = nullptr);
	TreeNode* GetRootTreeNode();
	TreeNode* GetTreeNode(GameObject* pgameObject);
	void DeleteGameObjectUsingIdentifier(string& identifier);
	void DeleteSelectedGameObject();

	//Getters
	string& GetSceneName();
};

