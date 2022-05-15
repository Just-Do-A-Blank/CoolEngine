#include "MeleeWeaponGameObject.h"

MeleeWeaponGameObject::MeleeWeaponGameObject(string identifier, CoolUUID uuid) : WeaponGameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::MELEE_WEAPON;
}

MeleeWeaponGameObject::MeleeWeaponGameObject(const nlohmann::json& data, CoolUUID uuid) : WeaponGameObject(data, uuid)
{
	m_gameObjectType |= GameObjectType::MELEE_WEAPON;

	m_swingAngle = data["SwingAngle"];
	m_chargeTime = data["ChargeTime"];
	m_radius = data["Radius"];
	m_isBlunt = data["IsBlunt"];
}

MeleeWeaponGameObject::~MeleeWeaponGameObject()
{
}

void MeleeWeaponGameObject::SetSwingAngle(float angle)
{
    m_swingAngle = angle;
}

void MeleeWeaponGameObject::SetChargeTime(float time)
{
    m_chargeTime = time;
}

void MeleeWeaponGameObject::SetRadius(float rad)
{
    m_radius = rad;
}

void MeleeWeaponGameObject::SetIsBlunt(bool blunt)
{
    m_isBlunt = blunt;
}

float MeleeWeaponGameObject::GetSwingAngle()
{
    return m_swingAngle;
}

float MeleeWeaponGameObject::GetChargeTime()
{
    return m_chargeTime;
}

float MeleeWeaponGameObject::GetRadius()
{
    return m_radius;
}

bool MeleeWeaponGameObject::GetIsBlunt()
{
    return m_isBlunt;
}

void MeleeWeaponGameObject::Serialize(nlohmann::json& data)
{
	WeaponGameObject::Serialize(data);

	data["SwingAngle"] = m_swingAngle;
	data["ChargeTime"] = m_chargeTime;
	data["Radius"] = m_radius;
	data["IsBlunt"] = m_isBlunt;
}
