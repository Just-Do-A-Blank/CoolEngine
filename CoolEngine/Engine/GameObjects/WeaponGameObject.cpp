#include "WeaponGameObject.h"

WeaponGameObject::WeaponGameObject(string identifier) : TriggerableGameObject(identifier)
{
    m_gameObjectType |= GameObjectType::WEAPON;
}
