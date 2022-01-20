#pragma once
#include "Engine/GameObjects/GameObject.h"

class CharacterGameObject : public GameObject
{
private:

	float m_moveSpeed = 100.0f;

	float m_health = 42.0f;

public:

	CharacterGameObject(string identifier);
	~CharacterGameObject();

	float GetMoveSpeed() { return m_moveSpeed; }
};