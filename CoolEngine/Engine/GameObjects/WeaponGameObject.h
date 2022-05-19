#pragma once
#include "Engine/GameObjects/TriggerableGameObject.h"
#include "Engine/Managers/Events/DamageCalculation.h"

class WeaponGameObject : public TriggerableGameObject
{
public:
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

	int GetLevel();
	int GetStrength();
	int GetDamage();
	ELEMENTS GetElement();
	STATUSES GetStatusEffect();
	int GetShotCount();
	float GetTimeLethal();
	float GetDistanceTravelled();
	bool GetIsPlayerWeapon();

	bool GetIsDualType();
	int RoundUp(float value);

private:
	int m_level = 0;
	int m_strength = 0;
	float m_damage = 1;

	int m_shotCount = 1;
	float m_timeLethal = 1;
	float m_distanceTravelled = 1;

	ELEMENTS m_element = ELEMENTS::NONE;
	STATUSES m_statusEffect = STATUSES::NONE;

	bool m_isPlayerWeapon = true;
};