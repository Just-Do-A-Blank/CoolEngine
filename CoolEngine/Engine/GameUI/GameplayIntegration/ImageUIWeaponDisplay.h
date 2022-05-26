#pragma once
#include "Engine/GameUI/GameplayIntegration/GameplayUIWeaponAttachment.h"

class ImageComponent;

class ImageUIWeaponDisplay : public GameplayUIWeaponAttachment
{
public:
    ImageUIWeaponDisplay(ImageComponent* imageComponent);
    ImageUIWeaponDisplay(nlohmann::json& data, ImageComponent* imageComponent);
    ImageUIWeaponDisplay(ImageUIWeaponDisplay const& other, ImageComponent* imageComponent);
    virtual ~ImageUIWeaponDisplay() override;

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
    /// <param name="weaponGameObject">The weapon to display</param>
    virtual void Update(WeaponGameObject* weaponGameObject) override;
private:
    /// <summary>
    /// The actual UI Image
    /// </summary>
    ImageComponent* m_imageComponent;

    void LoadLocalData(const nlohmann::json& jsonData);
    void SaveLocalData(nlohmann::json& jsonData);

    ID3D11ShaderResourceView* m_ptextureWeaponAttached = nullptr;
    ID3D11ShaderResourceView* m_ptextureWeaponNotAttached = nullptr;

    wstring m_texturePathAttached;
    wstring m_texturePathNotAttached;

    
};

