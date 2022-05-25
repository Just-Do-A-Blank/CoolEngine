#pragma once
#include "Engine/GameObjects/WeaponGameObject.h"

enum class BluntTypes
{
	CLUB = 0,
	BAT = 1,
	HAMMER = 2,
	CRUSHER = 3,
	ARM = 4,
	COUNT
};

enum class SharpTypes
{
	SWORD = 0,
	SHORTSWORD = 1,
	LONGBLADE = 2,
	CLEAVER = 3,
	AXE = 4,
	COUNT
};

class MeleeWeaponGameObject : public WeaponGameObject
{
public:
	MeleeWeaponGameObject(string identifier, CoolUUID uuid);
	MeleeWeaponGameObject(const nlohmann::json& data, CoolUUID uuid);
	MeleeWeaponGameObject(MeleeWeaponGameObject const& other);

	virtual ~MeleeWeaponGameObject()override;

	void SetSwingAngle(float angle);
	void SetChargeTime(float time);
	void SetRadius(float rad);
	void SetIsBlunt(bool blunt);
	void SetSwingTime(float time);

	float GetSwingAngle();
	float GetChargeTime();
	float GetRadius();
	bool GetIsBlunt();
	float GetSwingTime();

	virtual void Serialize(nlohmann::json& data) override;

protected:
    virtual void LoadAllPrefabData(const nlohmann::json& jsonData) override;
    virtual void SaveAllPrefabData(nlohmann::json& jsonData) override;

private:
	float m_swingAngle = 90.0f;
	float m_chargeTime = 0.5f;
	float m_radius = 50.0f;
	float m_swingTime = 0.5f;
	
	bool m_isBlunt = false;

    void LoadLocalData(const nlohmann::json& jsonData);
    void SaveLocalData(nlohmann::json& jsonData);
};