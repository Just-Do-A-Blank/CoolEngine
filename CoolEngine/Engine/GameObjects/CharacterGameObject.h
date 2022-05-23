#pragma once
#include "TriggerableGameObject.h"
#include "Engine/Managers/Events/DamageCalculation.h"

class WeaponGameObject;

class CharacterGameObject : public TriggerableGameObject
{
protected:

	ELEMENTS m_element = ELEMENTS::NONE;
	STATUSES m_status = STATUSES::NONE;
	ELEMENTALSTATUSES m_elementalStatus = ELEMENTALSTATUSES::NONE;

	float m_moveSpeed = 100.0f;
	float m_health = 2.0f;
	float m_invincibilityTime = 0.0f;

	XMFLOAT3 m_direction = XMFLOAT3{0, 0, 0};

	WeaponGameObject* m_pweapon = nullptr;

    virtual void LoadAllPrefabData(const nlohmann::json& jsonData) override;
    virtual void SaveAllPrefabData(nlohmann::json& jsonData) override;

public:

	CharacterGameObject();
	CharacterGameObject(string identifier, CoolUUID uuid);
	CharacterGameObject(const nlohmann::json& data, CoolUUID uuid);
	CharacterGameObject(CharacterGameObject const& other);
	virtual ~CharacterGameObject()override;

	virtual void Update();
	virtual void EditorUpdate();

	WeaponGameObject* GetWeapon();

	float GetMoveSpeed() { return m_moveSpeed; }
	float GetInvincibilityTime() { return m_invincibilityTime; }
	ELEMENTS GetElement() { return m_element; }
	STATUSES GetStatus() { return m_status; }
	ELEMENTALSTATUSES GetElementalStatus() { return m_elementalStatus; }
	void SetSpeed(float speed) { m_moveSpeed = speed; }
	void SetInvincibilityTime(float time) { m_invincibilityTime = time; }
	void SetElement(ELEMENTS elem) { m_element = elem; }
	void SetStatus(STATUSES stat) { m_status = stat; }
	void SetElementalStatus(ELEMENTALSTATUSES elemStat) { m_elementalStatus = elemStat; }

	void TakeDamage(float damage);

	virtual void Serialize(nlohmann::json& jsonData) override;

private:
    void LoadLocalData(const nlohmann::json& jsonData);
    void SaveLocalData(nlohmann::json& jsonData);
};
