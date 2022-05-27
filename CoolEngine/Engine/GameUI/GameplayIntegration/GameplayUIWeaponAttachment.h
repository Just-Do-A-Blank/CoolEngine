#pragma once
#include "Engine\Includes\json.hpp"
#include "Engine/GameObjects/PlayerGameObject.h"
#include "Engine/GameObjects/WeaponGameObject.h"
#include "Engine\GameUI\GameplayIntegration\EWeaponUIAttachmentOption.h"
#include "Engine\Managers\Events\EventObserver.h"
#include "Engine\Managers\Events\PickupEvent.h"

class GameplayUIWeaponAttachment : public Observer
{
public:
    GameplayUIWeaponAttachment();
    GameplayUIWeaponAttachment(nlohmann::json& data);
    GameplayUIWeaponAttachment(GameplayUIWeaponAttachment const& other);
    virtual ~GameplayUIWeaponAttachment();

#if EDITOR
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
    void Update();

    /// <summary>
    /// Handles events from the Observations
    /// </summary>
    void Handle(Event* e) override;

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

    /// <summary>
    /// The player resource attached to this player
    /// </summary>
    EWEAPONUIATTACHMENTOPTION m_weaponAttachmentOption;

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
    /// The current weapon key
    /// </summary>
    string m_weaponKey;

    /// <summary>
    /// True means we have updated the UI
    /// </summary>
    bool m_haveEverUpdatedTheUI;

    /// <summary>
    /// Detirmines if we should update the UI
    /// </summary>
    /// <param name="weapon">Current weapon to display</param>
    /// <returns>True means we should</returns>
    bool ShouldUpdateUI(WeaponGameObject* weapon);

    /// <summary>
    /// Call after updating the UI
    /// </summary>
    /// <param name="weapon">Current weapon to display</param>
    void UpdatedUI(WeaponGameObject* weapon);

    /// <summary>
    /// Handles pickup events
    /// </summary>
    void PlayerPickedupItem(PickupEvent* e);

    /// <summary>
    /// True means we are safe to scan the inventory for a new weapon
    /// </summary>
    bool m_playerPickedUpItem;

    /// <summary>
    /// Gets the weapon in the inventory
    /// </summary>
    /// <returns>The weapon to use</returns>
    WeaponGameObject* GetWeaponInInventory();

    /// <summary>
    /// Weapon currently in the players inventory. Can be nullptr.
    /// </summary>
    WeaponGameObject* m_weaponCurrentlyInInventory;

#if EDITOR
    list<pair<int, string>> m_attachmentSettingList;

    pair<int, string> m_attachmentSettingSelectedItem;

    list<pair<int, string>> GetattachmentSettingsAsList();

    pair<int, string> GetattachmentSettingsFromIndex(int index);
#endif
};