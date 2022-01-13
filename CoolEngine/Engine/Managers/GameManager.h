#pragma once
#include "Engine/GameObjects/GameObject.h"
#include "Engine/Helpers/Timer.h"
#include "Engine/Structure/Singleton.h"

class GameObject;

class GameManager : Singleton<GameManager>
{
	unordered_map<string, GameObject*> m_gameObjectMap;

	Timer m_timer;

public:

	GameObject* GetGameObject(string name);

	Timer* GetTimer();
};

