#pragma once
#include "Engine/GameObjects/GameObject.h"

class GameObject;

class GameManager
{
	unordered_map<string, GameObject*> m_gameObjectMap;
};

