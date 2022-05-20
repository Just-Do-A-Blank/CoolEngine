#pragma once
#include "Engine/GameObjects/GameObject.h"

class PrefabGameObject : virtual public GameObject
{
public:
    PrefabGameObject();
    PrefabGameObject(PrefabGameObject const& other);
    PrefabGameObject(string identifier, CoolUUID uuid);
    PrefabGameObject(const nlohmann::json& data, CoolUUID uuid);

    virtual ~PrefabGameObject()override;

    virtual void Serialize(nlohmann::json& jsonData) override;

#if EDITOR
    virtual void ShowEngineUI();
#endif

protected:

    virtual void LoadPrefabData(const nlohmann::json& jsonData) {};

private:

    void ValidateKeyAndType();

    /// <summary>
    /// The identifier for the prefab
    /// </summary>
    string m_prefabKey;

    /// <summary>
    /// The identifier for the prefab
    /// </summary>
    GameObjectType m_prefabType;
};

