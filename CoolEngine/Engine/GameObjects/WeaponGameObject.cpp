#include "WeaponGameObject.h"

WeaponGameObject::WeaponGameObject(string identifier, CoolUUID uuid) : TriggerableGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::WEAPON;
}

WeaponGameObject::WeaponGameObject(json data, CoolUUID index) : TriggerableGameObject(data, index)
{
    m_gameObjectType |= GameObjectType::WEAPON;
}

WeaponGameObject::~WeaponGameObject()
{
}

void WeaponGameObject::CalculateWeaponStrength()
{
    m_strength = ((float)GetIsDualType() + 0.5f) * (m_statusEffect == STATUSES::NONE ? 0.5f : 1.5f) * RoundUp( (10.0f * m_distanceTravelled / m_timeLethal) * m_damage * m_shotCount );
}

void WeaponGameObject::SetLevel(int lvl)
{
    m_level = lvl;
}

void WeaponGameObject::SetStrength(int strength)
{
    m_strength = strength;
}

void WeaponGameObject::SetDamage(int dam)
{
    m_damage = dam;
}

void WeaponGameObject::SetElement(ELEMENTS element)
{
    m_element = element;
}

void WeaponGameObject::SetStatusEffect(STATUSES effect)
{
    m_statusEffect = effect;
}

void WeaponGameObject::SetShotCount(int count)
{
    m_shotCount = count;
}

void WeaponGameObject::SetTimeLethal(float time)
{
    m_timeLethal = time;
}

void WeaponGameObject::SetDistanceTravelled(float dist)
{
    m_distanceTravelled = dist;
}

int WeaponGameObject::GetLevel()
{
    return m_level;
}

int WeaponGameObject::GetStrength()
{
    return m_strength;
}

int WeaponGameObject::GetDamage()
{
    return m_damage;
}

ELEMENTS WeaponGameObject::GetElement()
{
    return m_element;
}

STATUSES WeaponGameObject::GetStatusEffect()
{
    return m_statusEffect;
}

int WeaponGameObject::GetShotCount()
{
    return m_shotCount;
}

float WeaponGameObject::GetTimeLethal()
{
    return m_timeLethal;
}

float WeaponGameObject::GetDistanceTravelled()
{
    return m_distanceTravelled;
}

bool WeaponGameObject::GetIsDualType()
{
    return ((int)m_element >= (int)ELEMENTS::POISONFIRE);
}

int WeaponGameObject::RoundUp(float value)
{
    int temp = (int)value;
    if ((float)temp != value)
    {
        // If not a whole number already
        return (temp + 1);
    }

    return temp;
}
