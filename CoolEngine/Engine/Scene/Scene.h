#pragma once
#include "Engine/Managers/GraphicsManager.h"

class GameObject;

class Scene
{
private:
	string m_sceneIdentifier;

	GameObject* m_pcurrentlySelectedGameObject = nullptr;

	unordered_map<string, GameObject*> m_gameObjectMap;
public:
	Scene(string identifier);
	~Scene();

	virtual void Update();
	//virtual void Render(RenderStruct& renderStruct);

	unordered_map<string, GameObject*>& GetAllGameObjects();
	GameObject* GetGameObjectUsingIdentifier(string& identifier);
	void SelectGameObjectUsingIdentifier(string& identifier);

	void CreateGameObject(string& identifier);
	void DeleteGameObjectUsingIdentifier(string& identifier);
	void DeleteSelectedGameObject();

	//Getters
	string& GetSceneName();
};

