#pragma once
#include "Engine/GameObjects/GameObject.h"
#include "Engine/Helpers/Timer.h"
#include "Engine/Structure/Singleton.h"
#include "Engine/Scene/Scene.h"

class GameObject;

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

	GameObject* GetGameObject(string name);

	Timer* GetTimer();

	unordered_map<string, Scene*> m_sceneMap;
	Scene* m_pcurrentScene;

	void Update();
	void Render(RenderStruct& renderStruct);

	void CreateScene(string sceneIdentifier);
	void ChangeScene(string sceneIdentifier);
	void DeleteScene(string sceneIdentifier);

	void CreateGameObject(string identifier);
	void DeleteGameObject(string identifier);
};

