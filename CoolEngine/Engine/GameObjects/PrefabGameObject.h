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
    virtual void CreateEngineUI() override;
#endif

protected:

    virtual void CachePrefabData(string key);
    virtual void LoadPrefabData(const nlohmann::json& jsonData) {};
    virtual void SavePrefabData(nlohmann::json& jsonData);

    virtual void InitialPrefabLoad() {}
    nlohmann::json GetDataLoadedAtCreation();

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

    nlohmann::json m_prefabFileData;

    void LoadPrefab(string key);

    void SavePrefab(string key);
};

