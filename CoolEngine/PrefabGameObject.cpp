#include "Engine/GameObjects/PrefabGameObject.h"

PrefabGameObject::PrefabGameObject() : GameObject()
{

}

PrefabGameObject::PrefabGameObject(PrefabGameObject const& other)
{

}

PrefabGameObject::PrefabGameObject(string identifier, CoolUUID uuid)
{

}

PrefabGameObject::PrefabGameObject(const nlohmann::json& data, CoolUUID uuid) : GameObject(data, uuid)
{
    m_prefabKey = data["PrefabKey"];
    m_prefabType = data["PrefabType"];
    ValidateKeyAndType();

    if (m_prefabKey != "")
    {
        LoadPrefabData(data);
    }
}

PrefabGameObject::~PrefabGameObject()
{

}

void PrefabGameObject::Serialize(nlohmann::json& jsonData)
{
    GameObject::Serialize(jsonData);

    jsonData["PrefabKey"] = m_prefabKey;
}

void PrefabGameObject::ValidateKeyAndType()
{
    if (m_prefabType != m_gameObjectType)
    {
        m_prefabKey = "";
        m_prefabType = GameObjectType::BASE;
    }
}

#if EDITOR
    void PrefabGameObject::ShowEngineUI()
    {

    }
#endif