#include "InteractableGameObject.h"

InteractableGameObject::InteractableGameObject(string identifier, CoolUUID uuid) : TriggerableGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::INTERACTABLE;
}

InteractableGameObject::InteractableGameObject(const json& data, CoolUUID index) : TriggerableGameObject(data, index)
{
    m_gameObjectType |= GameObjectType::INTERACTABLE;
}

InteractableGameObject::~InteractableGameObject()
{
}

void InteractableGameObject::Serialize(json& jsonData)
{
	TriggerableGameObject::Serialize(jsonData);
}
