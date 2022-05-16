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

void RangedWeaponGameObject::SetSpeed(float speed)
{
	m_shotSpeed = speed;
}

float RangedWeaponGameObject::GetAngleInterval()
{
	return m_angleInterval;
}

bool RangedWeaponGameObject::GetIsShot()
{
	return m_isShot;
}

float RangedWeaponGameObject::GetSpeed()
{
	return m_shotSpeed;
}

void RangedWeaponGameObject::Serialize(nlohmann::json& data)
{
	WeaponGameObject::Serialize(data);

	data["AngleInterval"] = m_angleInterval;
	data["ShotSpeed"] = m_shotSpeed;
}
