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

class WeaponGameObject : public TriggerableGameObject
{
public:
	WeaponGameObject(string identifier, CoolUUID uuid);
	virtual ~WeaponGameObject()override;

	void SetLevel(int lvl);
	void SetStrength(int strength);
	void SetDamage(int dam);
	void SetElement(int element1, int element2);
	void SetStatusEffect(int effect);

	int GetLevel();
	int GetStrength();
	int GetDamage();
	int GetElement();
	int GetStatusEffect();

private:
	int m_level = 0;
	int m_strength = 0;
	float m_damage = 0;

	Elements m_element = Elements::NONE;
	Statuses m_statusEffect = Statuses::NONE;
};