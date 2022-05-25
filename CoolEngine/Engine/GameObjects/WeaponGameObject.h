#pragma once
#include "Engine/GameObjects/TriggerableGameObject.h"
#include "Engine/Managers/Events/DamageCalculation.h"
#include "Engine/ResourceDefines.h"

class WeaponGameObject : public TriggerableGameObject, public Observer
{
public:
	WeaponGameObject();
	WeaponGameObject(string identifier, CoolUUID uuid);
	WeaponGameObject(const nlohmann::json& data, CoolUUID uuid);
	WeaponGameObject(WeaponGameObject const& other);
	virtual ~WeaponGameObject()override;

	virtual void Serialize(nlohmann::json& data) override;

	// Calculates the overall strength stat from the other stats
	void CalculateWeaponStrength();

	void SetLevel(int lvl);
	void SetStrength(int strength);
	void SetDamage(int dam);
	void SetElement(ELEMENTS element);
	void SetStatusEffect(STATUSES effect);
	void SetShotCount(int count);
	void SetTimeLethal(float time);
	void SetDistanceTravelled(float dist);
	void SetIsPlayerWeapon(bool player);
	void SetBulletTexturePath(wstring path);
	void SetBulletScale(XMFLOAT3 scale);
	void SetCollisionScale(XMFLOAT2 scale);

	int GetLevel();
	int GetStrength();
	int GetDamage();
	ELEMENTS GetElement();
	STATUSES GetStatusEffect();
	int GetShotCount();
	float GetTimeLethal();
	float GetDistanceTravelled();
	bool GetIsPlayerWeapon();
	wstring GetBulletTexturePath();
	XMFLOAT3 GetBulletScale();
	XMFLOAT2 GetCollisionScale();

	bool GetIsDualType();
	int RoundUp(float value);

	void Handle(Event* e) override;

	void RegisterForEvents();
	void UnregisterForEvents();

	virtual void Attack();

protected:
    virtual void LoadAllPrefabData(const nlohmann::json& jsonData) override;
    virtual void SaveAllPrefabData(nlohmann::json& jsonData) override;

private:
	int m_level = 0;
	int m_strength = 0;
	float m_damage = 1;

	int m_shotCount = 1;
	float m_timeLethal = 1;
	float m_distanceTravelled = 100.0f;

	wstring m_bulletTexturePath = DEFAULT_IMGUI_IMAGE;
	XMFLOAT3 m_bulletScale = XMFLOAT3(25, 25, 25);
	XMFLOAT2 m_collisionScale = XMFLOAT2(1, 1);

	ELEMENTS m_element = ELEMENTS::NONE;
	STATUSES m_statusEffect = STATUSES::NONE;

	bool m_isPlayerWeapon = true;

    void LoadLocalData(const nlohmann::json& jsonData);
    void SaveLocalData(nlohmann::json& jsonData);
};