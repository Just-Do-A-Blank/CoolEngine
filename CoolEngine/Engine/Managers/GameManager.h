#pragma once
#include "Engine/Helpers/Timer.h"
#include "Engine/Structure/Singleton.h"

class GameObject;
class Scene;
class TreeNode;
class PlayerGameObject;
class RenderStruct;

enum class SceneDesc
{
	SCENE_1,
	SCENE_2,
	TEST_SCENE,
	SIZE
};

class GameManager : public Singleton<GameManager>
{
	
private:

	Timer m_timer;

public:
	Timer* GetTimer();

	unordered_map<string, Scene*> m_sceneMap;
	Scene* m_pcurrentScene;

	void Update();
	void Render(RenderStruct& renderStruct);

	void CreateScene(string sceneIdentifier);
	void SelectScene(Scene* psnene);
	void SelectSceneUsingIdentifier(string sceneIdentifier);
	void DeleteScene(Scene* pscene);
	void DeleteSceneUsingIdentifier(string sceneIdentifier);
	void DeleteSelectedScene();

	Scene* GetCurrentScene();


	unordered_map<string, GameObject*>& GetAllGameObjects();
	GameObject* GetGameObjectUsingIdentifier(string& identifier);
	PlayerGameObject* GetPlayerGameObjectUsingIdentifier(string& identifier);

	void SelectGameObjectUsingIdentifier(string& identifier);
	void SelectGameObject(GameObject* pgameObject);
	void SelectGameObjectUsingTreeNode(TreeNode* pnode);

	GameObject* CreateGameObject(string identifier);
	GameObject* CreatePlayerGameObject(string identifier);

	void DeleteSelectedGameObject();
	void DeleteGameObjectUsingIdentifier(string identifier);

	TreeNode* GetRootTreeNode();
	TreeNode* GetTreeNode(GameObject* pgameObject);
	string& GetCurrentSceneName();


	//Getters
	unordered_map<string, Scene*> GetSceneList();
	unordered_map<string, GameObject*>& GetAllGameObjectsInCurrentScene();
};

