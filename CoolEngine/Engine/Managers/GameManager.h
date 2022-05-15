#pragma once
#include "Engine/Helpers/Timer.h"
#include "Engine/Structure/Manager.h"
#include "Engine/Scene/Scene.h"

class GameObject;
class Scene;
template<class T>class TreeNode;
class RenderStruct;
class CameraGameObject;


enum class SceneDesc
{
	SCENE_1,
	SCENE_2,
	TEST_SCENE,
	SIZE
};

class GameManager : public Manager<GameManager>
{
	
private:

	Timer m_timer;

	string m_workingDirectory = "";
	wstring m_wideWorkingDirectory = L"";

	CameraGameObject* m_pcamera = nullptr;

public:
	void Init();

	Timer* GetTimer();

	unordered_map<string, Scene*> m_sceneMap;
	Scene* m_pcurrentScene;
	Scene* m_pplayScene;

	void Update();
	void Render(RenderStruct& renderStruct);

	void CreateScene(string sceneIdentifier);
	void SelectScene(Scene* psnene);
	void SelectSceneUsingIdentifier(string sceneIdentifier);
	void DeleteScene(Scene* pscene);
	void DeleteSceneUsingIdentifier(string sceneIdentifier);
	void DeleteSelectedScene();

	void BeginPlay();
	void EndPlay();

	string GetWorkingDirectory();
	wstring GetWideWorkingDirectory();

	Scene* GetCurrentScene();

	GameObject* GetSelectedGameObject();

	vector<GameObject*>& GetAllGameObjects();

	CameraGameObject* GetCamera();
	void SetCamera(CameraGameObject* pcamera);

	template<typename T>
	T* GetGameObjectUsingIdentifier(string& identifier)
	{
		return m_pcurrentScene->GetGameObjectUsingIdentifier<T>(identifier);
	}

	template<typename T>
	T* CreateGameObject(string identifier, TreeNode<GameObject>* nodeParent = nullptr)
	{
		return m_pcurrentScene->CreateGameObject<T>(identifier, nodeParent);
	}

	void DeleteGameObjectUsingNode(TreeNode<GameObject>* currentNode);
	void DeleteGameObjectUsingIdentifier(string identifier);

	template<typename T>
	void DeleteGameObjectUsingNode(T* pgameObject, std::string identifier)
	{
		m_pcurrentScene->DeleteGameObjectUsingNode(pgameObject, identifier);
	}

	TreeNode<GameObject>* GetRootTreeNode();
	TreeNode<GameObject>* GetTreeNode(GameObject* pgameObject);
	string& GetCurrentSceneName();


	//Getters
	unordered_map<string, Scene*> GetSceneList();
	vector<GameObject*>& GetAllGameObjectsInCurrentScene();

	void Serialize(nlohmann::json& data) override;
	void Deserialize(nlohmann::json& data) override;
};

