#pragma once

#include <stack>
#include "Engine/Helpers/Timer.h"
#include "Engine/Structure/Manager.h"
#include "Engine/Scene/Scene.h"
#include "Engine/GameObjects/EnemyGameObject.h"
#include "Engine/Physics/ParticleSystem.h"
#include "Engine/Managers/AudioManager.h"
#include "Engine/Managers/FontManager.h"


#include <sstream>
#include <cstdint> 

class GameObject;
class Scene;
template<class T>class TreeNode;
class RenderStruct;
class CameraGameObject;
class ParticleSystem;
class EnemyGameObject;
class BulletCreator;

enum class SceneDesc
{
	SCENE_1,
	SCENE_2,
	TEST_SCENE,
	SIZE
};

enum class ViewState
{
	EDITOR_VIEW,
	GAME_VIEW,
};

class GameManager : public Manager<GameManager>
{
	
private:

	Timer m_timer;

	string m_workingDirectory = "";
	wstring m_wideWorkingDirectory = L"";

	CameraGameObject* m_peditorCamera = nullptr;

	// Observer for making attacks
	BulletCreator* m_pbulletCreator = nullptr;

	ViewState m_viewState = ViewState::EDITOR_VIEW;

public:
	void Init();

	Timer* GetTimer();

	unordered_map<string, Scene*> m_editorSceneMap;
	unordered_map<string, Scene*> m_gameSceneMap;
	Scene* m_pcurrentEditorScene;
	Scene* m_pcurrentGameScene;

	void Start();
	void Update();
	void Render(RenderStruct& renderStruct);

	void CreateScene(string, bool unloadCurrentScene = false);
	bool LoadSceneFromFile(std::string fileLocation, bool unloadCurrentScene = false);
	void SwitchScene(Scene* psnene);
	bool SwitchSceneUsingIdentifier(string sceneIdentifier);
	void DeleteScene(Scene* pscene);
	void DeleteSceneUsingIdentifier(string sceneIdentifier);
	void DeleteSelectedScene();
	void SwitchAndDeleteScene(string sceneIdentifier);

	bool BeginPlay();
	bool EndPlay();
	void CopyScene();
	ViewState GetViewState()const;

	string GetWorkingDirectory();
	wstring GetWideWorkingDirectory();

	Scene* GetCurrentScene();

	GameObject* GetSelectedGameObject();

	vector<GameObject*>& GetAllGameObjects();

	CameraGameObject* GetCamera();
	void SetActiveCameraUsingIdentifier(string identifier);

	template<typename T>
	T* GetGameObjectUsingIdentifier(string& identifier)
	{
		switch (m_viewState)
		{
		case ViewState::EDITOR_VIEW:
			return m_pcurrentEditorScene->GetGameObjectUsingIdentifier<T>(identifier);
			break;

		case ViewState::GAME_VIEW:
			return m_pcurrentGameScene->GetGameObjectUsingIdentifier<T>(identifier);
			break;
		}
		
	}

	template<typename T>
	T* CreateGameObject(string identifier, TreeNode<GameObject>* nodeParent = nullptr)
	{
		return GetCurrentViewStateScene()->CreateGameObject<T>(identifier, nodeParent);
	}

	void DeleteGameObjectUsingNode(TreeNode<GameObject>* currentNode);
	void DeleteGameObjectUsingIdentifier(string identifier);

	template<typename T>
	void DeleteGameObjectUsingNode(T* pgameObject, std::string identifier)
	{
		return GetCurrentViewStateScene()->DeleteGameObjectUsingNode(pgameObject, identifier);
	}

	TreeNode<GameObject>* GetRootTreeNode();
	TreeNode<GameObject>* GetTreeNode(GameObject* pgameObject);
	string& GetCurrentSceneName();

	//Getters
	unordered_map<string, Scene*> GetSceneList();
	vector<GameObject*>& GetAllGameObjectsInCurrentScene();

	void Serialize(nlohmann::json& data) override;
	void Deserialize(nlohmann::json& data)override;

	unordered_map<string, Scene*>& GetCurrentViewStateSceneMap();
	Scene*& GetCurrentViewStateScene();
};

