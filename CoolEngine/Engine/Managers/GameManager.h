#pragma once
#include "Engine/GameObjects/GameObject.h"
#include <map>
#include <string>

class GameObject;

class GameManager
{
	map<string, GameObject*> m_gameObjectMap;
};

