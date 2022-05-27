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
    /// Run at the start of the game to set default values
    /// </summary>
    virtual void Start();

    /// <summary>
    /// Updates the resources for draining
    /// </summary>
    virtual void Update(float timeDelta);

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

    /// <summary>
    /// Uses a resource
    /// </summary>
    /// <param name="key">Resource to use</param>
    /// <param name="value">Amount to use</param>
    /// <returns>True means it was used, False means it could not be used</returns>
    bool UseResource(string key, int value);

    /// <summary>
    /// Gives a resource
    /// </summary>
    /// <param name="key">Resource to give</param>
    /// <param name="value">Amount to give</param>
    /// <returns>True means it was given, False means it could not be given</returns>
    bool GiveResource(string key, int value);

    /// <summary>
    /// Gets the value of a resource
    /// </summary>
    /// <param name="key">Keyt of the resource</param>
    /// <returns>The value or -1 if not found</returns>
    int GetResourceValue(string key);

private:
    map<string, PlayerResource*> m_resources;

    /// <summary>
    /// Time passed in the update loop. (Used simalarly to a fixed update)
    /// </summary>
    float m_timePassed;

    /// <summary>
    /// Update for the regeneration cycle
    /// </summary>
    void RegenerationUpdate();

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

