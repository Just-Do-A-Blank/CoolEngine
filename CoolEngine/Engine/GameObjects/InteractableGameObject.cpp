#include "InteractableGameObject.h"

InteractableGameObject::InteractableGameObject(string identifier) : TriggerableGameObject(identifier)
{
    m_gameObjectType |= GameObjectType::INTERACTABLE;
}
