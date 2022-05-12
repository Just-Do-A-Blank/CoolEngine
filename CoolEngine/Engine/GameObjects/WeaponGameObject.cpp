#include "WeaponGameObject.h"

WeaponGameObject::WeaponGameObject(string identifier, CoolUUID uuid) : TriggerableGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::WEAPON;
}

WeaponGameObject::~WeaponGameObject()
{
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

void WeaponGameObject::SetElement(int element1, int element2)
{
    m_element = (element1 | element2);
}

void WeaponGameObject::SetStatusEffect(int effect)
{
    m_statusEffect = effect;
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

int WeaponGameObject::GetElement()
{
    return m_element;
}

int WeaponGameObject::GetStatusEffect()
{
    return m_statusEffect;
}
