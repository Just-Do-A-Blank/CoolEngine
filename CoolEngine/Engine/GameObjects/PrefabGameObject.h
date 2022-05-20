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
    virtual void LoadAllPrefabData(const nlohmann::json& jsonData) {};
    virtual void SaveAllPrefabData(nlohmann::json& jsonData);

    nlohmann::json GetPrefabDataLoadedAtCreation();

    /// <summary>
    /// True, means this is a prefab
    /// </summary>
    bool IsPrefab();

private:
    /// <summary>
    /// The identifier for the prefab
    /// </summary>
    string m_prefabKey;

    nlohmann::json m_prefabFileData;

    void LoadPrefab(string key);

    void SavePrefab(string key);

    virtual void CachePrefabData(string key);

    //void LoadAllLocalData(const nlohmann::json& jsonData);
    void SaveAllLocalData(nlohmann::json& jsonData);
};

