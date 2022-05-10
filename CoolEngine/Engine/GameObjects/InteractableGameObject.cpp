#include "InteractableGameObject.h"

InteractableGameObject::InteractableGameObject(string identifier, CoolUUID uuid) : TriggerableGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::INTERACTABLE;
}