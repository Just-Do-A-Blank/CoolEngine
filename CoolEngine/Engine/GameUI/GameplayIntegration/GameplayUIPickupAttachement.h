#pragma once
#include "Engine/Includes/json.hpp"
#include "Engine/Managers/Events/EventObserver.h"
#include "Engine\Managers\Events\PickupEvent.h"

class PickupGameObject;
class PlayerGameObject;

class GameplayUIPickupAttachement : public Observer
{
public:
    GameplayUIPickupAttachement();
    GameplayUIPickupAttachement(nlohmann::json& data);
    GameplayUIPickupAttachement(GameplayUIPickupAttachement const& other);
    virtual ~GameplayUIPickupAttachement();

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
    PickupGameObject* GetPickupToDisplay();

    /// <summary>
    /// Runs during gameplay with the resource value
    /// </summary>
    /// <param name="pickupGameObject">The pickup to display</param>
    virtual void Update(PickupGameObject* pickupGameObject) {}

    /// <summary>
    /// The player resource attached to this player
    /// </summary>
    int m_pickupSlot;

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
    /// The current pickup key
    /// </summary>
    string m_pickupKey;

    /// <summary>
    /// True means we have updated the UI
    /// </summary>
    bool m_haveEverUpdatedTheUI;

    /// <summary>
    /// True mean player has picked up item so we should up date our information
    /// </summary>
    bool m_playerPickedUpItem;

    /// <summary>
    /// Current pickup
    /// </summary>
    PickupGameObject* m_currentPickup;

    /// <summary>
    /// Handles pickup events
    /// </summary>
    void PlayerPickedupItem(PickupEvent* e);
};

