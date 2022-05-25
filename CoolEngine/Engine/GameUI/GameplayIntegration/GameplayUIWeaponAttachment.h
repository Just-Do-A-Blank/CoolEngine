#pragma once
#include "Engine\Includes\json.hpp"
//#include "Engine/GameObjects/PlayerGameObject.h"
//#include "Engine/GameObjects/WeaponGameObject.h"
//#include "Engine\GameUI\GameplayIntegration\EWeaponUIAttachmentOption.h"

class GameplayUIWeaponAttachment
{
public:
    GameplayUIWeaponAttachment();
    GameplayUIWeaponAttachment(nlohmann::json& data);
    GameplayUIWeaponAttachment(GameplayUIWeaponAttachment const& other);
    virtual ~GameplayUIWeaponAttachment();

/*#if EDITOR
    virtual void CreateEngineUI();

    /// <summary>
    /// A label for when the player is found
    /// </summary>
    string m_buttonFoundPlayerLabel;
#endif

    virtual void Serialize(nlohmann::json& data);

    /// <summary>
    /// Called after construction, before first Update.
    /// </summary>
    virtual void Start();

    /// <summary>
    /// True means an inital attempt to find the player was completed
    /// </summary>
    bool m_attmptedToFindPlayer;

    /// <summary>
    /// Attempts to find the player once at the start of the objects life
    /// </summary>
    /// <param name="force">True means it will overide the start of life</param>
    void AttemptToFindPlayer(bool force = false);

    /// <summary>
    /// Called when the UI element should update
    /// </summary>
    virtual void Update();

    void LoadFromTopLevel(const nlohmann::json& jsonData);
    void SaveFromTopLevel(nlohmann::json& jsonData);

protected:
    virtual void LoadAllPrefabData(const nlohmann::json& jsonData);
    virtual void SaveAllPrefabData(nlohmann::json& jsonData);

    /// <summary>
    /// Gets the weapon used in the UI for this object
    /// </summary>
    /// <returns>The weapon to use</returns>
    WeaponGameObject* GetWeaponGameObject();


    /// <summary>
    /// Runs during gameplay with the resource value
    /// </summary>
    /// <param name="weaponGameObject">The weapon to display</param>
    virtual void Update(WeaponGameObject* weaponGameObject) {}

private:
    void LoadLocalData(const nlohmann::json& jsonData);
    void SaveLocalData(nlohmann::json& jsonData);

    /// <summary>
    /// Gets the player gameobject. Safe to call in editor or play.
    /// </summary>
    /// <returns>The current player</returns>
    PlayerGameObject* GetPlayer();

    /// <summary>
    /// The current player. In game this is the current player.
    /// </summary>
    PlayerGameObject* m_currentPlayer;

    /// <summary>
    /// The player resource attached to this player
    /// </summary>
    EWEAPONUIATTACHMENTOPTION m_weaponAttachmentOption;

#if EDITOR
    list<pair<int, string>> m_attachmentSettingList;

    pair<int, string> m_attachmentSettingSelectedItem;

    list<pair<int, string>> GetattachmentSettingsAsList();

    pair<int, string> GetattachmentSettingsFromIndex(int index);
#endif*/
};


