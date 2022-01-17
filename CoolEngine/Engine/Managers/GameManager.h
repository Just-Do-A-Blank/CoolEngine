#pragma once
#include "Engine/Helpers/Timer.h"
#include "Engine/Structure/Singleton.h"

class GameObject;
class Scene;

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
	//void Render(RenderStruct& renderStruct);

	void CreateScene(string sceneIdentifier);
	void ChangeScene(string sceneIdentifier);
	void DeleteScene(string sceneIdentifier);

	void CreateGameObject(string identifier);
	void DeleteGameObject(string identifier);

	//Getters
	unordered_map<string, Scene*> GetSceneList();
	unordered_map<string, GameObject*>& GetAllGameObjectsInCurrentScene();
};

