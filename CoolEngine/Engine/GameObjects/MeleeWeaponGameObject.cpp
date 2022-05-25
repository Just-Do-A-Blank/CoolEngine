#include "MeleeWeaponGameObject.h"

MeleeWeaponGameObject::MeleeWeaponGameObject(string identifier, CoolUUID uuid) : WeaponGameObject(identifier, uuid)
{
	m_gameObjectType |= GameObjectType::MELEE_WEAPON;
}

MeleeWeaponGameObject::MeleeWeaponGameObject(const nlohmann::json& data, CoolUUID uuid) : WeaponGameObject(data, uuid)
{
	m_gameObjectType |= GameObjectType::MELEE_WEAPON;

    if (PrefabGameObject::IsPrefab())
    {
        LoadLocalData(PrefabGameObject::GetPrefabDataLoadedAtCreation());
    }
    else
    {
        LoadLocalData(data);
    }
}

MeleeWeaponGameObject::MeleeWeaponGameObject(MeleeWeaponGameObject const& other) : WeaponGameObject(other)
{
	m_swingAngle = other.m_swingAngle;
	m_chargeTime = other.m_chargeTime;
	m_radius = other.m_radius;
	m_isBlunt = other.m_isBlunt;
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

void MeleeWeaponGameObject::SetSwingTime(float time)
{
    m_swingTime = time;
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

float MeleeWeaponGameObject::GetSwingTime()
{
    return m_swingTime;
}

void MeleeWeaponGameObject::Serialize(nlohmann::json& data)
{
	WeaponGameObject::Serialize(data);

    SaveLocalData(data);
}

void MeleeWeaponGameObject::LoadLocalData(const nlohmann::json& jsonData)
{
    if (jsonData.contains("SwingAngle"))
    {
        m_swingAngle = jsonData["SwingAngle"];
        m_chargeTime = jsonData["ChargeTime"];
        m_radius = jsonData["Radius"];
        m_isBlunt = jsonData["IsBlunt"];
    }
}

void MeleeWeaponGameObject::SaveLocalData(nlohmann::json& jsonData)
{
    jsonData["SwingAngle"] = m_swingAngle;
    jsonData["ChargeTime"] = m_chargeTime;
    jsonData["Radius"] = m_radius;
    jsonData["IsBlunt"] = m_isBlunt;
}

void MeleeWeaponGameObject::LoadAllPrefabData(const nlohmann::json& jsonData)
{
    LoadLocalData(jsonData);
    WeaponGameObject::LoadAllPrefabData(jsonData);
}

void MeleeWeaponGameObject::SaveAllPrefabData(nlohmann::json& jsonData)
{
    SaveLocalData(jsonData);
    WeaponGameObject::SaveAllPrefabData(jsonData);
}
