#pragma once
#include "Engine/GameObjects/WeaponGameObject.h"


class BulletGameObject : public WeaponGameObject
{
public:
	BulletGameObject();
	BulletGameObject(string identifier, CoolUUID uuid);
	BulletGameObject(const nlohmann::json& data, CoolUUID uuid);
	virtual ~BulletGameObject()override;

	virtual void Serialize(nlohmann::json& data) override;

	void Update() override;

	XMFLOAT3 GetDirection() { return m_direction; }
	float GetSpeed() { return m_speed; }

	void SetDirection(XMFLOAT3 dir) { m_direction = dir; }
	void SetSpeed(float speed) { m_speed = speed; }

private:
	XMFLOAT3 m_direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float m_speed = 1.0f;
};