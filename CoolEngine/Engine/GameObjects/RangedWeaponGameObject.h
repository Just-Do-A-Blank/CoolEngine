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
	RangedWeaponGameObject(const nlohmann::json& data, CoolUUID uuid);
	RangedWeaponGameObject(RangedWeaponGameObject const& other);

	virtual ~RangedWeaponGameObject()override;
	
	void SetAngleInterval(float angle);
	void SetIsShot(bool shot);
	void SetSpeed(float speed);

	float GetAngleInterval();
	bool GetIsShot();
	float GetSpeed();

	virtual void Serialize(nlohmann::json& data) override;

private:
	float m_angleInterval = 0.0f;
	float m_shotSpeed = 50.0f;

	bool m_isShot = false;
};