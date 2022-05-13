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
	virtual ~MeleeWeaponGameObject()override;

	void SetSwingAngle(float angle);
	void SetChargeTime(float time);
	void SetRadius(float rad);
	void SetIsBlunt(bool blunt);

	float GetSwingAngle();
	float GetChargeTime();
	float GetRadius();
	bool GetIsBlunt();

private:
	float m_swingAngle = 0.0f;
	float m_chargeTime = 0.0f;
	float m_radius = 0.0f;
	
	bool m_isBlunt = false;
};