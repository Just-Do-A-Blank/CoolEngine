#pragma once
#include "Engine/GameObjects/GameObject.h"

class GameObject;

class GameManager
{
	map<string, GameObject*> m_gameObjectMap;
};

