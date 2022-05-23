#pragma once
#include "Engine/GameObjects/Gameplay/Player/PlayerResource.h"
#include "Engine/Includes/json.hpp"
#include "Engine/GameObjects/Gameplay/Player/PlayerResourceInterface.h"

/// <summary>
/// Manages the Player Resources (Health and Stamina for example)
/// </summary>
class PlayerResourceManager
{
public:
	PlayerResourceManager();
	PlayerResourceManager(const nlohmann::json& data);
    PlayerResourceManager(PlayerResourceManager const& other);
	virtual ~PlayerResourceManager();

    /// <summary>
    /// Saves the player resources
    /// </summary>
    /// <param name="name">Data to add to</param>
    void SaveData(nlohmann::json& jsonData);

    /// <summary>
    /// Loads the player resources
    /// </summary>
    /// <param name="name">Data to load from</param>
    void LoadData(const nlohmann::json& jsonData);

#if EDITOR
    /// <summary>
    /// Shows engine UI
    /// </summary>
    virtual void CreateEngineUI();

    /// <summary>
    /// Gets all the keys in the resource list
    /// </summary>
    /// <returns>All the keys for resources</returns>
    list<string> GetResourceKeys();
#endif

    /// <summary>
    /// Checks to see if the player is dead
    /// </summary>
    /// <returns>True means player is alive, false means player is dead</returns>
    bool CheckForPlayerDeath();

    /// <summary>
    /// Takes damage if Weapon is ticked
    /// </summary>
    /// <param name="damage">Damage amount</param>
    void TakeWeaponDamage(float damage);

private:
    map<string, PlayerResource*> m_resources;

#if EDITOR
    PlayerResourceInterface* m_resourceInterface;

    /// <summary>
    /// All the keys in resource
    /// </summary>
    list<string> m_resourceKeys;

    /// <summary>
    /// Update key list. This should be called when the key list is updated.
    /// </summary>
    void UpdateAllKeys();
#endif
};

