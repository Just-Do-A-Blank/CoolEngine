#pragma once
#include "GameUIComponent.h"

class GameplayUIResourceAttachment;
class ImageUIResourceDisplay;
class GameplayUIWeaponAttachment;

class ImageComponent : public GameUIComponent
{
	friend FileIO;
public:
    ImageComponent(string identifier, CoolUUID uuid);
	ImageComponent(nlohmann::json& data, CoolUUID uuid);
	ImageComponent(ImageComponent const& other);
    virtual ~ImageComponent() override;

#if EDITOR
		void CreateEngineUI() override;
#endif

    virtual void Start() override;
    virtual void Update() override;
    virtual void Serialize(nlohmann::json& jsonData) override;

protected:
    /// <summary>
    /// Load all prefab data - this should be overriden and passed up the chain to save the whole prefab.
    /// </summary>
    /// <param name="jsonData">Data to use</param>
    virtual void LoadAllPrefabData(const nlohmann::json& jsonData);

    /// <summary>
    /// Save all prefab data - this should be overriden and passed up the chain to save the whole prefab.
    /// </summary>
    /// <param name="jsonData">Data to add to</param>
    virtual void SaveAllPrefabData(nlohmann::json& jsonData);

private:

    /// <summary>
    /// Saves all local data within this class
    /// </summary>
    /// <param name="jsonData">Data to be added to</param>
    void SaveAllLocalData(nlohmann::json& jsonData);

    /// <summary>
    /// Loads all local data within this class
    /// </summary>
    /// <param name="jsonData">Data to be loaded</param>
    void LoadAllLocalData(const nlohmann::json& jsonData);

	/// <summary>
	/// The resource attached to the component
	/// </summary>
	GameplayUIResourceAttachment* m_resourceAttachement;

    /// <summary>
    /// The image version of the Resource. ONLY USED IN THE COPY. PLEASE USE m_resourceAttachement.
    /// </summary>
    ImageUIResourceDisplay* m_resourceAttachementImage;

    GameplayUIWeaponAttachment* m_weaponAttachement;
};

