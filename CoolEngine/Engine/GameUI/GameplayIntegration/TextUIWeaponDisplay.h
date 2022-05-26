#pragma once
#include "Engine/GameUI/GameplayIntegration/GameplayUIWeaponAttachment.h"
#include "Engine/GameUI/GameplayIntegration/EWeaponTextDisplayStat.h"

class TextComponent;

class TextUIWeaponDisplay : public GameplayUIWeaponAttachment
{
public:
    TextUIWeaponDisplay(TextComponent* textComponent);
    TextUIWeaponDisplay(nlohmann::json& data, TextComponent* textComponent);
    TextUIWeaponDisplay(TextUIWeaponDisplay const& other, TextComponent* textComponent);
    virtual ~TextUIWeaponDisplay() override;

#if EDITOR
    virtual void CreateEngineUI() override;
#endif

    virtual void Serialize(nlohmann::json& data) override;

    /// <summary>
    /// Called after construction, before first Update.
    /// </summary>
    virtual void Start() override;

protected:
    virtual void LoadAllPrefabData(const nlohmann::json& jsonData) override;
    virtual void SaveAllPrefabData(nlohmann::json& jsonData) override;

    /// <summary>
    /// Runs during gameplay with the resource value
    /// </summary>
    /// <param name="weaponGameObject">The weapon to display</param>
    virtual void Update(WeaponGameObject* weaponGameObject) override;
private:
    void LoadLocalData(const nlohmann::json& jsonData);
    void SaveLocalData(nlohmann::json& jsonData);

    /// <summary>
    /// Which text based stat to display
    /// </summary>
    EWEAPONTEXTDISPLAYSTAT m_displayStat;

    /// <summary>
    /// What to display if there is no weapon
    /// </summary>
    string m_displayWhenNoWeapon;

    /// <summary>
    /// The actual component
    /// </summary>
    TextComponent* m_textComponent;

#if EDITOR
    list<pair<int, string>> m_WeaponStatList;

    pair<int, string> m_WeaponStatSelectedItem;

    list<pair<int, string>> GetWeaponStatAsList();

    pair<int, string> GetWeaponStatFromIndex(int index);
#endif
};