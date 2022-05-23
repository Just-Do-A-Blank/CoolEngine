#include "InteractableGameObject.h"

InteractableGameObject::InteractableGameObject(string identifier, CoolUUID uuid) : TriggerableGameObject(identifier, uuid)
{
    m_gameObjectType |= GameObjectType::INTERACTABLE;
}

InteractableGameObject::InteractableGameObject(const nlohmann::json& data, CoolUUID index) : TriggerableGameObject(data, index)
{
    m_gameObjectType |= GameObjectType::INTERACTABLE;

    if (PrefabGameObject::IsPrefab())
    {
        LoadLocalData(PrefabGameObject::GetPrefabDataLoadedAtCreation());
    }
    else
    {
        LoadLocalData(data);
    }
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

    SaveLocalData(jsonData);
}

void InteractableGameObject::LoadLocalData(const nlohmann::json& jsonData)
{

}

void InteractableGameObject::SaveLocalData(nlohmann::json& jsonData)
{

}

void InteractableGameObject::LoadAllPrefabData(const nlohmann::json& jsonData)
{
    LoadLocalData(jsonData);
    TriggerableGameObject::LoadAllPrefabData(jsonData);
}

void InteractableGameObject::SaveAllPrefabData(nlohmann::json& jsonData)
{
    SaveLocalData(jsonData);
    TriggerableGameObject::SaveAllPrefabData(jsonData);
}