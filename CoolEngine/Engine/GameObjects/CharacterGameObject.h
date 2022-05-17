#pragma once
#include "TriggerableGameObject.h"
#include "Engine/GameObjects/DamageCalculation.h"

class CharacterGameObject : public TriggerableGameObject
{
protected:

	float m_moveSpeed = 100.0f;

	float m_health = 42.0f;

	XMFLOAT3 m_direction = XMFLOAT3{0, 0, 0};

public:

	CharacterGameObject();
	CharacterGameObject(string identifier, CoolUUID uuid);
	CharacterGameObject(const nlohmann::json& data, CoolUUID uuid);
	CharacterGameObject(CharacterGameObject const& other);
	virtual ~CharacterGameObject()override;

	virtual void Update();
	virtual void EditorUpdate();

	float GetMoveSpeed() { return m_moveSpeed; }
	void SetSpeed(float speed) { m_moveSpeed = speed; }

	virtual void Serialize(nlohmann::json& jsonData) override;
};
