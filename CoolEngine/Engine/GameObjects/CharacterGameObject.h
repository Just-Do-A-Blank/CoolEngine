#pragma once
#include "Engine/GameObjects/GameObject.h"

class CharacterGameObject : public GameObject
{
private:

	float m_moveSpeed = 10.0f;

	float m_health = 42.0f;

public:

	CharacterGameObject(string identifier);
	~CharacterGameObject();

	void Translate(XMFLOAT2 vector);

	float GetMoveSpeed() { return m_moveSpeed; }
};