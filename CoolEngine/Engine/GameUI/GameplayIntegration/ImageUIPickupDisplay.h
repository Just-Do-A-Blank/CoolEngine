#pragma once
#include "Engine/GameUI/GameplayIntegration/GameplayUIPickupAttachement.h"

class ImageComponent;

class ImageUIPickupDisplay : public GameplayUIPickupAttachement
{
public:
    ImageUIPickupDisplay(ImageComponent* imageComponent);
    ImageUIPickupDisplay(nlohmann::json& data, ImageComponent* imageComponent);
    ImageUIPickupDisplay(ImageUIPickupDisplay const& other, ImageComponent* imageComponent);
    virtual ~ImageUIPickupDisplay() override;

#if EDITOR
    virtual void CreateEngineUI() override;
#endif

    virtual void Serialize(nlohmann::json& data) override;

    /// <summary>
    /// Called after construction, before first Update.
    /// </summary>
    virtual void Start() override;

    /// <summary>
    /// Set the texture when a weapon is displayed
    /// </summary>
    /// <param name="wsfilepath">Texture path - Blank is valid</param>
        void SetAttachedTexture(std::wstring wsfilepath);

    /// <summary>
    /// Set the texture when a weapon is not displayed
    /// </summary>
    /// <param name="wsfilepath">Texture path - Blank is valid</param>
    void SetNotAttachedTexture(std::wstring wsfilepath);

protected:
    virtual void LoadAllPrefabData(const nlohmann::json& jsonData) override;
    virtual void SaveAllPrefabData(nlohmann::json& jsonData) override;

    /// <summary>
    /// Runs during gameplay with the resource value
    /// </summary>
    /// <param name="pickupGameObject">The pickup to display</param>
    virtual void Update(PickupGameObject* pickupGameObject) override;

private:
    void LoadLocalData(const nlohmann::json& jsonData);
    void SaveLocalData(nlohmann::json& jsonData);

    /// <summary>
    /// The actual UI on the Image
    /// </summary>
    ImageComponent* m_imageComponent;

    ID3D11ShaderResourceView* m_ptextureWeaponAttached = nullptr;
    ID3D11ShaderResourceView* m_ptextureWeaponNotAttached = nullptr;

    wstring m_texturePathAttached;
    wstring m_texturePathNotAttached;
};

