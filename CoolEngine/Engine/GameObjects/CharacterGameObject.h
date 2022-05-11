#pragma once
#include "TriggerableGameObject.h"

class CharacterGameObject : public TriggerableGameObject
{
protected:

	float m_moveSpeed = 100.0f;

	float m_health = 42.0f;

	XMFLOAT3 m_direction = XMFLOAT3{0, 0, 0};

public:

	CharacterGameObject();
	CharacterGameObject(string identifier, CoolUUID uuid);
	virtual ~CharacterGameObject();

	virtual void Update();

	float GetMoveSpeed() { return m_moveSpeed; }
	void SetSpeed(float speed) { m_moveSpeed = speed; }


};
