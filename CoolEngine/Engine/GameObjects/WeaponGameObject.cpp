#include "WeaponGameObject.h"

WeaponGameObject::WeaponGameObject(string identifier, CoolUUID uuid) : TriggerableGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::WEAPON;
}

WeaponGameObject::WeaponGameObject(json& data, int index)
{
    m_gameObjectType |= GameObjectType::WEAPON;
}
