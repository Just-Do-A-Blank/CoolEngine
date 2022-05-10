#include "WeaponGameObject.h"

WeaponGameObject::WeaponGameObject() : TriggerableGameObject()
{
    m_gameObjectType |= GameObjectType::WEAPON;
}

WeaponGameObject::WeaponGameObject(string identifier) : TriggerableGameObject(identifier)
{
    m_gameObjectType |= GameObjectType::WEAPON;
}
