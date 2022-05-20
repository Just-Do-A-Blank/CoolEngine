#pragma once
#include "Engine/GameObjects/GameObject.h"

class PrefabGameObject : virtual public GameObject
{
public:
    PrefabGameObject();
    PrefabGameObject(PrefabGameObject const& other);
    PrefabGameObject(string identifier, CoolUUID uuid);
    PrefabGameObject(const nlohmann::json& data, CoolUUID uuid);
    virtual ~PrefabGameObject();

    virtual void Serialize(nlohmann::json& jsonData) override;

protected:
#if EDITOR
    virtual void CreateEngineUI() override;
#endif

    /// <summary>
    /// Gets the data loaded at launch. Used to update the object if it is a prefab.
    /// </summary>
    /// <returns>Prefab data</returns>
    const nlohmann::json GetPrefabDataFromLaunch();

    virtual void LoadAllPrefabData(const nlohmann::json& jsonData) {};
    virtual void SaveAllPrefabData(nlohmann::json& jsonData);
private:
    /// <summary>
    /// The identifier for the prefab
    /// </summary>
    string m_prefabKey;

    nlohmann::json m_prefabDataFromLaunch;

    void LoadPrefabDataAtLaunch(string key);

    void SaveLocalData(nlohmann::json& jsonData);

    void LoadPrefab(string key);

    void SavePrefab(string key);
};