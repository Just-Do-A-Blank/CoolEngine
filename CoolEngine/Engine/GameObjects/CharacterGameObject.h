#pragma once
#include "Engine/GameObjects/GameObject.h"

class CharacterGameObject : public GameObject
{
protected:

	float m_moveSpeed = 100.0f;

	float m_health = 42.0f;

	XMFLOAT3 m_direction = XMFLOAT3{0, 0, 0};

public:

	CharacterGameObject(string identifier);
	~CharacterGameObject();

	void Update();

	float GetMoveSpeed() { return m_moveSpeed; }
	void SetSpeed(float speed) { m_moveSpeed = speed; }


};