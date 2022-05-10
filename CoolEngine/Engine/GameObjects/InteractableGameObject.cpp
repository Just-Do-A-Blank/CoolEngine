#include "InteractableGameObject.h"

InteractableGameObject::InteractableGameObject() : TriggerableGameObject()
{
    m_gameObjectType |= GameObjectType::INTERACTABLE;
}

InteractableGameObject::InteractableGameObject(string identifier) : TriggerableGameObject(identifier)
{
    m_gameObjectType |= GameObjectType::INTERACTABLE;
}
