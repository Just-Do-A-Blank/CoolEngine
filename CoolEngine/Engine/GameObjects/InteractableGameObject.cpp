#include "InteractableGameObject.h"

InteractableGameObject::InteractableGameObject(string identifier, CoolUUID uuid) : TriggerableGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::INTERACTABLE;
}

InteractableGameObject::InteractableGameObject(const nlohmann::json& data, CoolUUID index) : TriggerableGameObject(data, index)
{
    m_gameObjectType |= GameObjectType::INTERACTABLE;
}

InteractableGameObject::InteractableGameObject(InteractableGameObject const& other) : TriggerableGameObject(other)
{
}

InteractableGameObject::~InteractableGameObject()
{
}

void InteractableGameObject::Serialize(nlohmann::json& jsonData)
{
	TriggerableGameObject::Serialize(jsonData);
}
