#include "MeleeWeaponGameObject.h"

MeleeWeaponGameObject::MeleeWeaponGameObject(string identifier, CoolUUID uuid) : WeaponGameObject(identifier, uuid)
{
    
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

void MeleeWeaponGameObject::SetSwingTime(float time)
{
    m_swingTime = time;
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

float MeleeWeaponGameObject::GetSwingTime()
{
    return m_swingTime;
}

float MeleeWeaponGameObject::GetRadius()
{
    return m_radius;
}

bool MeleeWeaponGameObject::GetIsBlunt()
{
    return m_isBlunt;
}
