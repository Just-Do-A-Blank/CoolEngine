#include "InteractableGameObject.h"

InteractableGameObject::InteractableGameObject(string identifier, CoolUUID uuid) : TriggerableGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::INTERACTABLE;
}

InteractableGameObject::InteractableGameObject(json& data, int index) : TriggerableGameObject(data, index)
{
    m_gameObjectType |= GameObjectType::INTERACTABLE;
}
