#pragma once
#include "Engine/GameObjects/TriggerableGameObject.h"

enum class Elements
{
	NONE = 0,
	FIRE = 0b1,
	WATER = 0b10,
	POISON = 0b100,
	ELECTRIC = 0b1000,
	COUNT
};

enum class Statuses
{
	NONE = 0,
	BLEEDING = 1,
	WEAKNESS = 2,
	CONFUSION = 3,
	ADRENALINE = 4,
	CHARM = 5,
	COUNT
};

DEFINE_ENUM_FLAG_OPERATORS(Elements);

class WeaponGameObject : public TriggerableGameObject
{
public:
	WeaponGameObject(string identifier, CoolUUID uuid);
	virtual ~WeaponGameObject()override;

	void CalculateWeaponStrength();

	void SetLevel(int lvl);
	void SetStrength(int strength);
	void SetDamage(int dam);
	void SetElement(Elements element1, Elements element2);
	void SetStatusEffect(Statuses effect);
	void SetShotCount(int count);
	void SetTimeLethal(float time);
	void SetDistanceTravelled(float dist);

	int GetLevel();
	int GetStrength();
	int GetDamage();
	Elements GetElement();
	Statuses GetStatusEffect();
	int GetShotCount();
	float GetTimeLethal();
	float GetDistanceTravelled();

	bool GetIsDualType();
	int RoundUp(float value);

private:
	int m_level = 0;
	int m_strength = 0;
	float m_damage = 0;

	int m_shotCount = 1;
	float m_timeLethal = 1;
	float m_distanceTravelled = 1;

	Elements m_element = Elements::NONE;
	Statuses m_statusEffect = Statuses::NONE;
};