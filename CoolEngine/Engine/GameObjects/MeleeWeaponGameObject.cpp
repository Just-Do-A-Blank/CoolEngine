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
