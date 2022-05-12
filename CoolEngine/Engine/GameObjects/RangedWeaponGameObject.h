#pragma once
#include "Engine/GameObjects/WeaponGameObject.h"

enum class ShotTypes
{
	BOW = 0,
	POTSHOT = 1,
	SPREADSHOT = 2,
	STRAIGHTSHOT = 3,
	ROD = 4,
	COUNT
};

enum class ThrownTypes
{
	BOOMERANG = 0,
	SPEAR = 1,
	STAFF = 2,
	KNIVES = 3,
	HARPOON = 4,
	COUNT
};

class RangedWeaponGameObject : public WeaponGameObject
{
public:
	RangedWeaponGameObject(string identifier, CoolUUID uuid);
	virtual ~RangedWeaponGameObject()override;

	void SetShotCount(int count);
	void SetAngleInterval(float angle);
	void SetIsShot(bool shot);

	int GetShotCount();
	float GetAngleInterval();
	bool GetIsShot();

private:
	int m_shotCount;
	float m_angleInterval;
	float m_shotSpeed;

	bool m_isShot;
};