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
	bool GetActive() { return m_isActive; }

	void SetDirection(XMFLOAT3 dir) { m_direction = dir; }
	void SetSpeed(float speed) { m_speed = speed; }
	void SetActive(bool active) { m_isActive = active; }
	void SetCurrentTime(float time) { m_currentTravelTime = time; }
	void SetTotalTime(float time) { m_totalTravelTime = time; }

private:
	XMFLOAT3 m_direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	float m_speed = 50.0f;

	float m_totalTravelTime = 1.0f;
	float m_currentTravelTime = 0.0f;
	bool m_isActive = false;
};