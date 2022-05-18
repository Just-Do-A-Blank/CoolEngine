#include "RangedWeaponGameObject.h"

RangedWeaponGameObject::RangedWeaponGameObject(string identifier, CoolUUID uuid) : WeaponGameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::RANGE_WEAPON;
}

RangedWeaponGameObject::RangedWeaponGameObject(const nlohmann::json& data, CoolUUID uuid) : WeaponGameObject(data, uuid)
{
	m_gameObjectType |= GameObjectType::RANGE_WEAPON;

	m_angleInterval = data["AngleInterval"];
	m_shotSpeed = data["ShotSpeed"];
}

RangedWeaponGameObject::RangedWeaponGameObject(RangedWeaponGameObject const& other) : WeaponGameObject(other)
{
	m_angleInterval = other.m_angleInterval;
	m_shotSpeed = other.m_shotSpeed;

	m_isShot = other.m_isShot;
}

RangedWeaponGameObject::~RangedWeaponGameObject()
{
}

void RangedWeaponGameObject::SetAngleInterval(float angle)
{
	m_angleInterval = angle;
}

void RangedWeaponGameObject::SetIsShot(bool shot)
{
	m_isShot = shot;
}

float RangedWeaponGameObject::GetAngleInterval()
{
	return m_angleInterval;
}

bool RangedWeaponGameObject::GetIsShot()
{
	return m_isShot;
}

void RangedWeaponGameObject::Serialize(nlohmann::json& data)
{
	WeaponGameObject::Serialize(data);

	data["AngleInterval"] = m_angleInterval;
	data["ShotSpeed"] = m_shotSpeed;
}
