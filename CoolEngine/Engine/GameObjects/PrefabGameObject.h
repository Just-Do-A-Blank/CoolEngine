#pragma once
#include "Engine/GameObjects/GameObject.h"
#include "Engine/EditorUI/EditorButtonCallback.h"

#define FOLDERLOCATIONPREFAB "//Resources//Prefabs"

/// <summary>
/// A basic prefab system which allows a single object to be serialized and loaded.
/// </summary>
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

    /// <summary>
    /// Called after construction, before first Update.
    /// </summary>
    virtual void Start() override;

protected:

    /// <summary>
    /// Load all prefab data - this should be overriden and passed up the chain to save the whole prefab.
    /// </summary>
    /// <param name="jsonData">Data to use</param>
    virtual void LoadAllPrefabData(const nlohmann::json& jsonData) {};

    /// <summary>
    /// Save all prefab data - this should be overriden and passed up the chain to save the whole prefab.
    /// </summary>
    /// <param name="jsonData">Data to add to</param>
    virtual void SaveAllPrefabData(nlohmann::json& jsonData);

    /// <summary>
    /// If this is a prefab, this gets the data to use 
    /// as opposed to the data saved by the scene which maybe out of date.
    /// Only used on CONSTRUCTION and is removed on START.
    /// </summary>
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

    /// <summary>
    /// The prefab key which was last loaded
    /// </summary>
    string m_prefabLoadedKey;

    /// <summary>
    /// True, means the none button error box is shown
    /// </summary>
    bool m_basicLoadErrorBox;

    /// <summary>
    /// True, means the none button error box is shown
    /// </summary>
    bool m_basicSaveErrorBox;

    /// <summary>
    /// True, means the button error box is shown
    /// </summary>
    bool m_buttonErrorBox;

    /// <summary>
    /// When using the two button error box, this gives the callbacks from the buttons
    /// </summary>
    EditorButtonCallback m_buttonMessageBoxCallback;

    /// <summary>
    /// The data gathered on creation if any
    /// </summary>
    nlohmann::json m_prefabFileData;

    /// <summary>
    /// Load the prefab file.
    /// Note this will not work on consutrction as construction is top down.
    /// </summary>
    /// <param name="key">The prefab file</param>
    void LoadPrefab(string key);

    /// <summary>
    /// Save the prefab to file
    /// </summary>
    /// <param name="key">The prefab key</param>
    void SavePrefab(string key);

    /// <summary>
    /// Stores the data given at launch. Used to allow polymorph children to grab prefab data.
    /// </summary>
    /// <param name="key">The prefab key</param>
    void CachePrefabData(string key);

    /// <summary>
    /// Saves all local data within this class
    /// </summary>
    /// <param name="jsonData">Data to be added to</param>
    void SaveAllLocalData(nlohmann::json& jsonData);

    /// <summary>
    /// Detirmines if the given key could be loaded if attempted - in theory
    /// </summary>
    /// <param name="key">The key (file name no extention)</param>
    /// <returns>True, means it could be</returns>
    bool CanLoadPrefab(string key);

    /// <summary>
    /// Detirmines if the prefab file at the location looks acceptable
    /// </summary>
    /// <param name="location">The actual location of the prefab file</param>
    /// <returns>True means valid</returns>
    bool IsPrefabFileValid(string location);

    /// <summary>
    /// Ensures the given string is a valid file name for a prefab (no extention)
    /// </summary>
    /// <param name="text">Text to alter</param>
    void PrepareUserInputAsPrefabFileName(std::string& s);

    /// <summary>
    /// Trims any space characters on either side of the string
    /// </summary>
    /// <param name="text">Text to trim</param>
    void TrimSpacesEitherSideOfText(std::string& s);

    /// <summary>
    /// Converts string to lowercase
    /// </summary>
    /// <param name="text">Text to alter</param>
    void TextToLower(string& text);

    /// <summary>
    /// Detirmines if this were saved if the prefab is overriden or not
    /// </summary>
    /// <param name="key">Key which would be used when saving (the new key)</param>
    /// <returns>True, means the prefab key is new to this gameobject.</returns>
    bool IsThisKeyCurrentKeyOrNewKey(string key);
};

